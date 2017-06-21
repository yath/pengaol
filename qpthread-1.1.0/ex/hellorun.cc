#include "ex.h"
#include <qpthr/qp.h>

class HelloWorld: public QpRunnable {
    public:
	virtual void Main() {
		cout << "Hello World" << endl;
	}
};


int main()
{
	QpInit qp_init;
	HelloWorld hw;
	QpThread t(&hw);
	
	t.Start();
	t.Join();
	return 0;
}

