#ifndef HBASE_CLIENT_POOL_H
#define HBASE_CLIENT_POOL_H
/* 
 * File: hbase_client_pool.h
 * Description: hbase client pool
 * Author: Charles. 2017-2-8.
 * Mailto: charlesliu.cn.bj@gmail.com
 */

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/transport/TSocket.h>
#include "Hbase.h"
#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include <time.h>
#include "base_define.h"
#include "ini_file.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::hadoop::hbase::thrift;

typedef struct {
    boost::shared_ptr<TTransport> thrift_transport;
    boost::shared_ptr<TTransport> thrift_socket;
    boost::shared_ptr<TProtocol> thrift_protocol;
    HbaseClient *hbase_client;
} hbase_client_t;

class HbaseClientPool {
public:
    HbaseClientPool();
    ~HbaseClientPool();
    hbase_client_t *getClient();
    int init();
    void putClient(hbase_client_t *client);
    hbase_client_t *addClient();
    bool getPoolLock();
    bool isPoolFull();
    hbase_client_t *newClient();
    void freeClient(hbase_client_t *client);
private:
    sem_t _sem;
    pthread_mutex_t _mutex;
    vector<hbase_client_t *> _clients;
    queue<hbase_client_t *> _rdqueue;
private:
    /* pool config */
    char thrift_server[256];
    int thrift_port;
    int max_client_count;
    int min_client_count;
    struct timespec poollock_timeout;
    int socket_recv_timeout;
    int socket_send_timeout;
};

#endif
