#include "ex.h"
#include <qpthr/qp.h>


class PerThreadObject {
    public:
	PerThreadObject() {}
	void Invoke() {
		cout << "my ptr is " << this << endl;
	}
};

QpTSDT<PerThreadObject> tsd;


class TsdtThread: public QpThread {
    public:
	virtual ~TsdtThread() {Join();}
	virtual void Main() {
		tsd->Invoke();
		QpThread::Sleep(1);
	}
};


int main()
{
	QpInit qp_init;
	TsdtThread t1, t2, t3, t4;
	
	t1.Start();
	t2.Start();
	t3.Start();
	t4.Start();
	
	return 0;
}

