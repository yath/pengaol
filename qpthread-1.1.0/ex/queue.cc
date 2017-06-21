#include "ex.h"
#include <qpthr/qp.h>


typedef QpQueue< list< int > > MsgQueue;
static int verbose = 0;

class Producer: public QpThread
{
    private:
	int p_iter;
	MsgQueue *p_queue;
    public:
	Producer(MsgQueue *q, int iter): p_iter(iter), p_queue(q) {}
	~Producer() { Join();}
	virtual void Main() {
		int i;
		for (i = 0; i < p_iter; i++) {
			p_queue->Send(i);
			if (verbose) {
				cout << "p" << ThreadSeq() << " ";
				cout.flush();
				cout << i << " ";
				cout.flush();
			}
		}
	}
};

class Consumer: public QpThread
{
    private:
	MsgQueue *c_queue;
    public:
	Consumer(MsgQueue *q): c_queue(q) {}
	~Consumer() {Join();}
	virtual void Main() {
		int val;
		
		while (c_queue->Receive(&val)) {
			if (verbose) {
				cout << "c" << ThreadSeq() << " ";
				cout.flush();
			}
			cout << val << " ";
			cout.flush();
		}
	}
};


int main(int argc, char *argv[])
{
	QpInit qp_init;
	int c, i;
	int iter = 1000;
	int thr_pro = 1, thr_cons = 1;
	int queue_size = 1024;
	
	while ((c = getopt(argc, argv, "vi:p:s:c:")) != EOF) {
		switch (c) {
		    case 'v':
			verbose = 1;
			break;
		    case 'i':
			iter = atoi(optarg);
			break;
		    case 'p':
			thr_pro = atoi(optarg);
			break;
		    case 'c':
			thr_cons = atoi(optarg);
			break;
		    case 's':
			queue_size = atoi(optarg);
			break;
		}
	}
	typedef Producer *Producer_t;
	typedef Consumer *Consumer_t;

	MsgQueue queue(queue_size);
	
	Producer_t *producer = new Producer_t[thr_pro];
	Consumer_t *consumer = new Consumer_t[thr_cons];
	for (i = 0; i < thr_pro; i++) {
		producer[i] = new Producer(&queue, iter);
		producer[i]->Start();
	}
	for (i = 0; i < thr_cons; i++) {
		consumer[i] = new Consumer(&queue);
		consumer[i]->Start();
	}
	
	for (i = 0; i < thr_pro; i++) {
		producer[i]->Join();
		delete producer[i];
	}
	delete [] producer;
	queue.SendDone();
	for (i = 0; i < thr_cons; i++) {
		consumer[i]->Join();
		delete consumer[i];
	}
	delete [] consumer;
	cout << endl;
	cout << "end" << endl;

  // XJB added to omit warning "function should return a value"
  return 0;
}
