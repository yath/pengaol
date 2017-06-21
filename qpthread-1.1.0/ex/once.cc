#include "ex.h"
#include <qpthr/qp.h>

class Once: public QpOnce {
	virtual void Main() {
		cout << "thr seq = " << QpThread::CurrentSeq() << endl;
	}
};

Once once;


class OnceThread: public QpThread {
    public:
	virtual ~OnceThread() {Join();}
	virtual void Main() {
		once.Do();
	}
};


int main()
{
	QpInit qp_init;
	OnceThread t1, t2, t3, t4;
	
	t1.Start();
	t2.Start();
	t3.Start();
	t4.Start();
	
	return 0;
}

