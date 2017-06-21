
DESCRIPTION

XCounter - программа для X-Window, показывающая иформацию о IP
трафике на выбранном интерфейсе. Информация включает следующие:
        
        Количество переданных и полученных байт
	Время текущего соединения 
	Скорость за последние n секунд
	(n может быть задано пользователем)
	
XCounter показывает статистику следующих интерфейсов:
Ethernet, FDDI, ISDN, PLIP, SLIP/PPP.


TECHNICAL NOTES

Ядро должно иметь подержку файловой системы /proc.
Программа разработана для запуска на Linux X Window System.


DOCUMENTATION 

При запуске в командной строке указывается интерфейс, о котором необходима
информация  (пример XCounter -i eth0). Если интерфейс не указан, то по умолчанию 
XCounter показывает информацию о интерфейсе ppp0.
Для сброса таймера и счетчика байт используется клавиша R, а для выхода из 
программы Q. Клавишей A изменяется свойство always on top.
Чтобы вкл/выкл монитор загрузки процессора нажмите C.
Информацию о трафике со времени как интерфейс был "поднят" можно увидеть поместив 
указатель мыши на окно XCounter-а. 
Нажмите S, чтобы сохранить текущие настройки программы в файл $HOME/.XCounter.
Для ядер 2.0.3x XCounter показывает только количество пакетов и скорость пакетов/сек.


COPYING AND DISTRIBUTION

This software is open-source and is distributed under the terms of the GNU
General Public License.


FEEDBACK

If you find any errors please mail to
 m_rsi@usa.net

RSI's Page
 http://members.fortunecity.com/mrsi
