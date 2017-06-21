#include "ex.h"
#include <qpthr/qp.h>

class HelloWorld: public QpThread {
    public:
	~HelloWorld() {Join();}
	virtual void Main() {
		cout << "Hello World" << endl;
	}
};


int main()
{
	QpInit qp_init;
	HelloWorld ht;
	
	ht.Start();
	return 0;
}

