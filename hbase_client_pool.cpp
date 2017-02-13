/*
 * File: hbase_client_pool.cpp
 */
#include "hbase_client_pool.h"
#include "woo/log.h"

HbaseClientPool::HbaseClientPool() {
    read_profile_string("HBASE", "thrift_server", thrift_server, sizeof(thrift_server), "172.31.44.218", OTHER_CONFIG);
    thrift_port = read_profile_int("HBASE", "thrift_port", 9090, OTHER_CONFIG);
    max_client_count = read_profile_int("HBASE", "max_client_count", 30, OTHER_CONFIG);
    min_client_count = read_profile_int("HBASE", "min_client_count", 10, OTHER_CONFIG);
    poollock_timeout.tv_sec = read_profile_int("HBASE", "poollock_timeout_second", 0, OTHER_CONFIG);
    poollock_timeout.tv_nsec = 1000 * read_profile_int("HBASE", "poollock_timeout_usecond", 300000, OTHER_CONFIG);
    socket_recv_timeout = read_profile_int("HBASE", "socket_recv_timeout", 300, OTHER_CONFIG);
    socket_send_timeout = read_profile_int("HBASE", "socket_send_timeout", 300, OTHER_CONFIG);

    sem_init(&_sem, 0, min_client_count);
    pthread_mutex_init(&_mutex, NULL);
}

HbaseClientPool::~HbaseClientPool() {
    sem_destroy(&_sem);
    pthread_mutex_destroy(&_mutex);

    vector<hbase_client_t *>::iterator it = _clients.begin();
    for (; it != _clients.end(); ++it) {
        if (*it != NULL) {
            (*it)->thrift_transport->close();
            delete (*it)->hbase_client;
            delete (*it);
        }
    }
}

hbase_client_t *HbaseClientPool::getClient() {
    if (getPoolLock() == false) 
        return NULL;

    hbase_client_t *client = NULL;
    pthread_mutex_lock(&_mutex);
    if (!_rdqueue.empty()) {
        client = _rdqueue.front();
        _rdqueue.pop();
    } else {
        client = addClient();
    }
    pthread_mutex_unlock(&_mutex);

    return client;
}

int HbaseClientPool::init() {
    hbase_client_t *client;
    for (int i = 0; i < min_client_count; ++i) {
        client = newClient();
        if (client == NULL)
            return -1;

        _clients.push_back(client);
        _rdqueue.push(client);
    }

    return 0;
}

bool HbaseClientPool::getPoolLock() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        return false;

    ts.tv_sec += poollock_timeout.tv_sec;
    ts.tv_nsec += poollock_timeout.tv_nsec;

    int ret = 0;
    while ((ret = sem_timedwait(&_sem, &ts)) == -1 && errno == EINTR)
        continue;

    if (ret == -1) 
        return false;

    return true;
}

void HbaseClientPool::putClient(hbase_client_t *client) {
    pthread_mutex_lock(&_mutex);
    _rdqueue.push(client);
    sem_post(&_sem);
    pthread_mutex_unlock(&_mutex);
}

hbase_client_t *HbaseClientPool::newClient() {
    hbase_client_t *client = new hbase_client_t;
    client->thrift_socket = boost::shared_ptr<TTransport>(new TSocket(thrift_server, thrift_port));
    client->thrift_transport = boost::shared_ptr<TTransport>(new TFramedTransport(client->thrift_socket));
    client->thrift_protocol = boost::shared_ptr<TProtocol>(new TBinaryProtocol(client->thrift_transport));
    client->hbase_client = new HbaseClient(client->thrift_protocol);

    try {
        client->thrift_transport->open();
    } catch (TException &tx) {
        LOG_ERROR("cannot connect to thrift server. (%s)", tx.what());
        delete client->hbase_client;
        client->hbase_client = NULL;
        delete client;
        client = NULL;
        return NULL;
    }

    return client;
}

void HbaseClientPool::freeClient(hbase_client_t *client) {
    client->thrift_transport->close();
    delete client->hbase_client;
    pthread_mutex_lock(&_mutex);
    --min_client_count;
    pthread_mutex_unlock(&_mutex);
}

hbase_client_t *HbaseClientPool::addClient() {
    if (min_client_count == max_client_count)
        return NULL;

    pthread_mutex_lock(&_mutex);
    hbase_client_t *client = newClient();
    ++min_client_count;
    _clients.push_back(client);
    pthread_mutex_unlock(&_mutex);

    return client;
}

bool HbaseClientPool::isPoolFull() {
    return (min_client_count == max_client_count);
}


