#include "ex.h"
#include <qpthr/qp.h>
#include <qpthr/qp_work.h>


void HelloWorld()
{
	cout << "Hello World" << endl;
}

int main()
{
	QpInit qp_init;
	QpThreadPool tpool;

	QpWork_p0 *w = new QpWork_p0(&HelloWorld);
	tpool.Do(w);
	tpool.Wait();
	delete w;
	return 0;
}

