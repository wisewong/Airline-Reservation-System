// Flight.c : 定义应用程序的入口点。
//
#include "stdafx.h"


//订单
typedef struct OrderForm{
	TCHAR IdNum[32];									//订单用户身份证号
	int Order_Number;									//订单号
	int Tickets_Num;									//订票数量
	TCHAR Flight_Number[16];							//航班号
	TCHAR Departure[16];								//出发地
	TCHAR Destination[16];								//目的地
	TCHAR Date[16];										//出发日期
	TCHAR TakeOff_Time[16];								//起飞时间
	TCHAR Landing_Time[16];								//降落时间
	struct OrderForm *Next;								//所有订单链表next
	struct OrderForm *psgNext;							//用户订单链表next
}OrderForm;

//乘客订单链表
typedef struct PsgOrderLink{
	int OrderNum;										//订单数目
	OrderForm *head;									//头结点
	OrderForm *tail;									//尾结点
}PsgOrderLink;

//所有订单链表
typedef struct AllOrderLink{
	int AllOrderNum;									//所有订单数目
	OrderForm *head;									//头结点
	OrderForm *tail;									//尾结点
}AllOrderLink;

//乘客
typedef struct Passenger{
	TCHAR Name[16];								        //姓名
	TCHAR IdNum[32];									//身份证号码
	TCHAR PassWord[32];							    	//密码
	int TicketNum;										//订单数目
	PsgOrderLink OrderLink;								//用户所有订单
	struct Passenger *Next;
}Passenger;

//乘客链表
typedef struct PsgLinkList{
	int PsgNum;											//账户数量
	Passenger *head;									//头结点
	Passenger *tail;									//尾结点
}PsgLinkList;

//航班
typedef struct Flight{
	double Fare;										//票价
	int Seat_Number;									//座位数
	int Vacant_Seat;									//空余座位数
	TCHAR Discount[16];									//折扣
	TCHAR Flight_Number[16];							//航班号
	TCHAR Departure[16];								//出发地
	TCHAR Destination[16];								//目的地
	TCHAR Date[16];										//出发日期
	TCHAR TakeOff_Time[16];								//起飞时间
	TCHAR Landing_Time[16];								//降落时间
	struct Flight *Next;
}Flight;

//航班链表
typedef struct FlightLinkList{
	Flight *head;										//头结点
	Flight *tail;										//尾结点
	int Flight_Number;									//航班数目
}FilghtLinkList;


// 全局变量: 
HICON hIcon;
HINSTANCE hInst;										//当前实例
static TCHAR szBuffer[256];								//缓冲区
static PsgLinkList psglink;								//所有账户_链表
static Passenger *passenger;							//登陆账户信息
static AllOrderLink allorder;							//所有订单_链表
static FilghtLinkList flightlink;						//所有航班_链表



//函数声明
BOOL AccountLogIn(HWND);										//账户登陆
BOOL AccountRegister(HWND);										//注册账户
BOOL ReadFlightData(HWND);										//读入航班信息
BOOL ReadAccountData(HWND);										//读入账户资料
BOOL ReadAccountOrder(HWND, Passenger*);						//读入所有订单、账户订单
BOOL SearchFlight(HWND);										//查询航班
BOOL BookTickets(HWND);											//订票
BOOL _Book_Tickets(HWND, Flight*, int);							//订票
BOOL Recommend(HWND, Flight*, int);								//航班建议
BOOL ReturnTickets(HWND);										//退票
BOOL EntryFlight(HWND);											//录入航班
BOOL ModifyFlight(HWND);										//修改航班信息
BOOL PrintFlight(HWND, Flight*);								//输出航班信息
BOOL WriteFlightData(HWND);										//保存航班信息
BOOL WriteAccountData(HWND);									//保存账户资料
BOOL WriteOrderData(HWND);										//保存订单信息
BOOL CALLBACK LogInDlgProc(HWND, UINT, WPARAM, LPARAM);			//登陆窗口窗口过程
BOOL CALLBACK MainDlgProc(HWND, UINT, WPARAM, LPARAM);			//主界面窗口过程
BOOL CALLBACK NameDlgProc(HWND, UINT, WPARAM, LPARAM);			//获取新注册用户姓名窗口过程
BOOL CALLBACK FlightNumDlgProc(HWND, UINT, WPARAM, LPARAM);		//获取用户输入机票数量窗口过程
BOOL CALLBACK EntryFlightProc(HWND, UINT, WPARAM, LPARAM);		//录入航班窗口过程
BOOL CALLBACK ModifyFlightProc(HWND, UINT, WPARAM, LPARAM);		//修改航班信息窗口过程



//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{

	hInst = hInstance;
	InitCommonControls();
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_LOGINDLG), NULL, (DLGPROC)LogInDlgProc);
}//WinMain


//登陆窗口_窗口过程
BOOL CALLBACK LogInDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		//WM_INITDIALOG是当其对话框和子控件全部创建完毕，将要显示内容的时候发送的消息
		//因此可以在WM_INITDIALOG消息响应函数中添加对编辑框控件的初始化和修改
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		ReadAccountData(hwndDlg);					//读取账户、航班数据
		ReadFlightData(hwndDlg);					
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hwndDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LOGIN:
			AccountLogIn(hwndDlg);					//登陆
			break;
		case IDC_REGISTER:
			AccountRegister(hwndDlg);				//注册
			break;
		}//switch

	}//WM_COMMAND
		return TRUE;

	}//switch
	return FALSE;
}//LogInDlgProc（）


//主界面_窗口过程
BOOL CALLBACK MainDlgProc(HWND hMainDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hMainDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		//关闭程序时将信息保存												
		if (WriteOrderData(hMainDlg) && WriteAccountData(hMainDlg) && WriteFlightData(hMainDlg))
			EndDialog(hMainDlg, 0);
		else
			MessageBox(hMainDlg, TEXT("保存信息错误！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_SEARCH:
		{
			SearchFlight(hMainDlg);						//查找航班	
			break;
		}
		case IDC_TICKET:
		{
			BookTickets(hMainDlg);						//订票
			break;
		}
		case IDC_RETURNTICKET:
		{
			ReturnTickets(hMainDlg);					//退票
			break;
		}
		case IDC_ENTRY:
		{												//录入航班对话框
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ENTRY), NULL, (DLGPROC)EntryFlightProc);
			break;
		}
		case IDC_MODIFYFLIGHT:
		{												//修改航班信息对话框
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MODIFYFLT), NULL, (DLGPROC)ModifyFlightProc);
			break;
		}
		}//switch
	}//WM_COMMAND
		return TRUE;
	}//switch
	return FALSE;
}//MainDlgProc()


//获取用户输入机票数量_窗口过程
BOOL CALLBACK FlightNumDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{

	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}//WM_INITDIALOG
		return TRUE;

	case WM_CLOSE:
	{
		wsprintf(szBuffer, TEXT("\0"));
		EndDialog(hwndDlg, FALSE);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
			wsprintf(szBuffer, TEXT("\0"));
			GetDlgItemText(hwndDlg, IDC_TICKETNUM, szBuffer, 256);
			EndDialog(hwndDlg, TRUE);
			break;
		}//switch
	}//WM_COMMAND
		return TRUE;

	}//switch
	return FALSE;
}


//获取新注册用户姓名_窗口过程
BOOL CALLBACK NameDlgProc(HWND hNameDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hNameDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
		return TRUE;

	case WM_CLOSE:
	{
		wsprintf(passenger->Name, TEXT("\0"));
		EndDialog(hNameDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_GETNAMEOK:
		{
			GetDlgItemText(hNameDlg, IDC_GETNAME, passenger->Name, 16);
			EndDialog(hNameDlg, TRUE);
			break;
		}
		case IDC_GETNAMECANCEL:
		{
			wsprintf(passenger->Name, TEXT("\0"));
			EndDialog(hNameDlg, TRUE);
			break;
		}

		}//stitch

	}//WM_COMMAND
	return TRUE;

	}//stitch

	return FALSE;

}//NameDlgProc()


//录入航班_窗口过程
BOOL CALLBACK EntryFlightProc(HWND hEntryDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hEntryDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
	return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hEntryDlg, 0);
	}
	return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_ADDFLT:
		{
			EntryFlight(hEntryDlg);						//录入航班
			break;
		}
		case IDC_QUIT:
		{
			EndDialog(hEntryDlg, FALSE);
			break;
		}

		}//stitch

	}//WM_COMMAND
		return TRUE;

	}//stitch

	return FALSE;
}


//修改航班信息_窗口过程
BOOL CALLBACK ModifyFlightProc(HWND hModifyDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		if (hIcon)
			SendMessage(hModifyDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}
		return TRUE;

	case WM_CLOSE:
	{
		EndDialog(hModifyDlg, 0);
	}//WM_CLOSE
		return TRUE;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_MODIFY:
		{
			ModifyFlight(hModifyDlg);					//修改航班信息
			break;
		}
		case IDC_QUIT:
		{
			EndDialog(hModifyDlg, FALSE);
			break;
		}

		}//stitch

	}//WM_COMMAND
		return TRUE;
	}//stitch

	return FALSE;
}//ModifyFlightProc()


//读入账户信息
BOOL ReadAccountData(HWND hwndDlg){

	FILE *fp;

	passenger = (Passenger *)malloc(sizeof(Passenger));										//为登录账户分配内存
	if (!passenger){
		MessageBox(hwndDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
		return FALSE;
	}//if

	psglink.head = NULL;
	psglink.tail = NULL;
	psglink.PsgNum = 0;

	if ((fp = fopen(".\\AccountData.txt", "r+")) == NULL){		//打开文件
		MessageBox(hwndDlg, TEXT("账户文件读入错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
	}//if		

	while (!feof(fp)){
		Passenger *p = (Passenger *)malloc(sizeof(Passenger));
		if (!p){
			MessageBox(hwndDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if

		//读入账户姓名、身份证号、密码
		if (fscanf(fp, "%s%s%s", p->Name, p->IdNum, p->PassWord) == EOF){
			free(p); break;
		}

		p->Next = NULL;
		if (psglink.head == NULL)						//读入第一个账户信息时，头、尾结点均指向p
			psglink.head = p;
		else
			psglink.tail->Next = p;						//否则，尾结点Next指向p
		psglink.tail = p;								//尾结点指向p
		psglink.PsgNum++;								//已注册账户个数

	}//while
	fclose(fp);											//关闭文件
	return TRUE;
}//ReadAccountData(HWND)


//读入航班信息
BOOL ReadFlightData(HWND hwndDlg){

	int flag = 0;
	FILE *fp;

	flightlink.Flight_Number = 0;
	flightlink.head = NULL;
	flightlink.tail = NULL;

	if ((fp = fopen(".\\FlightData.txt", "r")) == NULL){		//打开文件
		MessageBox(hwndDlg, TEXT("航班文件读入错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		EndDialog(hwndDlg, 0);
	}//if

	while (!feof(fp)){
		Flight *p = (Flight *)malloc(sizeof(Flight));
		if (!p){
			MessageBox(hwndDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if
		//读入航班信息
		if (fscanf(fp, "%s%lf%s%d%d%s%s%s%s%s",
			p->Flight_Number,
			&p->Fare,
			p->Discount,
			&p->Seat_Number,
			&p->Vacant_Seat,
			p->Departure,
			p->Destination,
			p->Date,
			p->TakeOff_Time,
			p->Landing_Time) == EOF)
		{
			free(p);
			break;
		}
		p->Next = NULL;
		if (flightlink.head == NULL)					//添加至链表
			flightlink.head = p;
		else
			flightlink.tail->Next = p;
		flightlink.tail = p;
		flightlink.Flight_Number++;

	}//while
	fclose(fp);											//关闭文件
	return TRUE;
}//ReadFlightData()


//读取订单信息
BOOL ReadAccountOrder(HWND hwndDlg, Passenger *passenger){

	FILE *fp;
	allorder.AllOrderNum = 0;							//订单链表初始化
	allorder.head = NULL;
	allorder.tail = NULL;
	passenger->TicketNum = 0;							//乘客订单链表初始化
	passenger->OrderLink.head = NULL;
	passenger->OrderLink.tail = NULL;
	passenger->OrderLink.OrderNum = 0;

	if ((fp = fopen(".\\OrderForm.txt", "r")) == NULL){	//打开文件
		MessageBox(hwndDlg, TEXT("订单文件读入错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	while (!feof(fp)){
		OrderForm *p = (OrderForm *)malloc(sizeof(OrderForm));
		if (!p){
			MessageBox(hwndDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
			EndDialog(hwndDlg, 0);
			return FALSE;
		}//if
		if (fscanf(fp, "%s%d%d%s%s%s%s%s%s",			//读取订单信息
			p->IdNum,									
			&p->Order_Number,
			&p->Tickets_Num,
			p->Flight_Number,
			p->Departure,
			p->Destination,
			p->Date,
			p->TakeOff_Time,
			p->Landing_Time) == EOF)
		{
			free(p);
			break;
		}
		p->Next = NULL;
		p->psgNext = NULL;

		if (allorder.head == NULL){						//将结点添至订单链表末尾
			allorder.head = p;
		}
		else
			allorder.tail->Next = p;
		allorder.tail = p;
		allorder.AllOrderNum++;							//订单数目增加

		if (!lstrcmp(p->IdNum, passenger->IdNum)){		//若该订单为当前登录账户订单，则添至用户订单链表末尾
			if (passenger->OrderLink.head == NULL){		//将结点添至用户订单链表末尾
				passenger->OrderLink.head = p;
			}
			else
				passenger->OrderLink.tail->psgNext = p;
			passenger->OrderLink.tail = p;
			passenger->TicketNum += p->Tickets_Num;
			passenger->OrderLink.OrderNum++;			//订单数目增加
		}//if
	}//while
	fclose(fp);
	return TRUE;
}//ReadAccountOrder()


//登陆用户验证
BOOL AccountLogIn(HWND hwndDlg){

	int flag = 0;																//flag！=0则该用户已注册，flag==0则该用户尚未注册
	Passenger *p;

	GetDlgItemText(hwndDlg, IDC_IDEDIT, passenger->IdNum, 256);					//获取用户输入ID、密码
	GetDlgItemText(hwndDlg, IDC_PSWEDIT, passenger->PassWord, 256);				

	p = psglink.head;
	while (p){																	//在用户链表查找登陆用户ID
		if (!lstrcmp(passenger->IdNum, p->IdNum)){
			flag++;																//用户已注册
			if (!lstrcmp(passenger->PassWord, p->PassWord)){					//密码匹配，登陆成功
				lstrcpy(passenger->Name, p->Name);
				MessageBox(hwndDlg, TEXT("登陆成功，单击确定进入程序主界面"), TEXT("登陆成功"), MB_OK | MB_ICONINFORMATION);
				break;
			}//if
			else{																//密码错误，退出循环
				MessageBox(hwndDlg, TEXT("密码错误，请重新输入"), TEXT("密码错误"), MB_OK | MB_ICONERROR);
				return FALSE;
			}//else
		}//if
		p = p->Next;
	}//while
	if (!flag){																	//用户尚未注册
		if (lstrlen(passenger->PassWord) == 0)
			MessageBox(hwndDlg, TEXT("请输入密码！"), TEXT("信息"), MB_OK | MB_ICONINFORMATION);
		else
			MessageBox(hwndDlg, TEXT("该账户尚未注册，请先注册"), TEXT("信息"), MB_OK | MB_ICONINFORMATION);
	}//if
	else{																		//关闭登陆界面，弹出主界面
		EndDialog(hwndDlg, TRUE);
		if (ReadAccountOrder(hwndDlg, passenger))								//读取用户订单
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, (DLGPROC)MainDlgProc);
	}//else

	return TRUE;
}//AccountLogIn(HWND)


//用户注册
BOOL AccountRegister(HWND hwndDlg){

	GetDlgItemText(hwndDlg, IDC_IDEDIT, passenger->IdNum, 256);			//获取用户输入ID、密码
	GetDlgItemText(hwndDlg, IDC_PSWEDIT, passenger->PassWord, 256);		

	if (lstrlen(passenger->PassWord) == 0){
		MessageBox(hwndDlg, TEXT("请输入注册用户密码！"), TEXT("信息"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_NAMEDLG), NULL, (DLGPROC)NameDlgProc);	//获取新注册用户姓名

	if (lstrlen(passenger->Name) == 0)
		return FALSE;

	passenger->Next = NULL;

	if (psglink.head == NULL)										//将新注册账户资料添加至账户链表
		psglink.head = passenger;
	else
		psglink.tail->Next = passenger;								
	psglink.tail = passenger;										
	psglink.PsgNum++;												//用户数目增加
	passenger->TicketNum = 0;
	passenger->OrderLink.head = NULL;								//乘客订单链表初始化
	passenger->OrderLink.tail = NULL;
	passenger->OrderLink.OrderNum = 0;

	MessageBox(hwndDlg, TEXT("注册成功！请单击确定进入主界面"), TEXT("注册成功"), MB_OK | MB_ICONINFORMATION);

	EndDialog(hwndDlg, TRUE);													//关闭登陆界面

	DialogBox(hInst, MAKEINTRESOURCE(IDD_MAINDLG), NULL, (DLGPROC)MainDlgProc); //弹出主界面

	return TRUE;
}//AccountRegister(HWND)


//查找航班
BOOL SearchFlight(HWND hMainDlg){
	int flag1 = 0;													//if(flag1==0) 出发地、目的地查询航班，else：航班号查航班
	int flag2 = 0;
	Flight *p;
	TCHAR szBuffer[256] = { TEXT("\0") };
	TCHAR szDeparture[16], szDestination[16], szFltNum[16], szDate[16];

	HWND hEdit = GetDlgItem(hMainDlg, IDC_INFORMATION);

	GetDlgItemText(hMainDlg, IDC_DEPARTURE, szDeparture, 15);		//获取用户输入出发地
	GetDlgItemText(hMainDlg, IDC_DESTINATION, szDestination, 15);	//获取用户输入目的地
	GetDlgItemText(hMainDlg, IDC_FLIGHTNUM, szFltNum, 15);			//获取用户输入航班号
	GetDlgItemText(hMainDlg, IDC_DATE, szDate, 15);					//获取用户选择航班时间

	SetDlgItemText(hMainDlg, IDC_INFORMATION, szBuffer);			//清空航班信息区域文本


	if (lstrlen(szFltNum) || (!(lstrlen(szDeparture)) && !(lstrlen(szDestination))))
		flag1++;
	else if (!(lstrlen(szDeparture))){
		MessageBox(hMainDlg, TEXT("请输入出发地！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	else if (!(lstrlen(szDestination))){
		MessageBox(hMainDlg, TEXT("请输入目的地！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	if (flag1 && !(lstrlen(szFltNum))){
		MessageBox(hMainDlg, TEXT("请输入出发地、目的地或航班号以查询航班！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	p = flightlink.head;
	while (p){											//查找航班并输出
		if (flag1){										//按航班号查找
			if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){
				if (!flag2){							//查找到符合航班，输出
					_stprintf(szBuffer, TEXT("航班号\t出发地\t目的地\t起飞时间\t降落时间\t余座\t价格\t折扣\t出发日期\n"));
					SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
					SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				}//if
				flag2++;
				PrintFlight(hMainDlg, p);				
			}//if
		}//if
		else{											//按出发地、目的地查找
			if (!lstrcmp(p->Departure, szDeparture) && !lstrcmp(p->Destination, szDestination) && !lstrcmp(p->Date, szDate)){
				if (!flag2){							//查找到符合航班，输出
					_stprintf(szBuffer, TEXT("航班号\t出发地\t目的地\t起飞时间\t降落时间\t余座\t价格\t折扣\t出发日期\n"));
					SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
					SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				}//if
				flag2++;
				PrintFlight(hMainDlg, p);				
			}//if
		}//else
		p = p->Next;
	}//while
	if (!flag2)
		MessageBox(hMainDlg, TEXT("对不起，没有符合您要求的航班"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	return TRUE;
}//SearchFlight()


//将所查航班信息输出
BOOL PrintFlight(HWND hwndDlg, Flight *p){
	TCHAR szBuffer[256];
	HWND hEdit = GetDlgItem(hwndDlg, IDC_INFORMATION);

	_stprintf(szBuffer, TEXT("\n%s\t%s\t%s\t%s\t%s\t%d\t%.2lf\t%s\t%s\n\n"),
		p->Flight_Number,				//航班号
		p->Departure,					//出发地
		p->Destination,					//目的地
		p->TakeOff_Time,				//起飞时间
		p->Landing_Time,				//降落时间
		p->Vacant_Seat,					//余座
		p->Fare,						//价格
		p->Discount,					//折扣
		p->Date);						//出发日期

	SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));

	return TRUE;
}


//订票
BOOL BookTickets(HWND hMainDlg){
	int tickets_num;
	Flight *p;
	TCHAR szFltNum[16], szDate[16];

	GetDlgItemText(hMainDlg, IDC_FLIGHTNUM, szFltNum, 15);			//获取用户输入航班号、航班日期
	GetDlgItemText(hMainDlg, IDC_DATE, szDate, 15);					

	if (!lstrlen(szFltNum)){
		MessageBox(hMainDlg, TEXT("请输入航班号以订票！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_FLTNUM), NULL, (DLGPROC)FlightNumDlgProc);	//获取订票数目

	if (!lstrlen(szBuffer))
		return FALSE;

	tickets_num = _ttoi(szBuffer);									//订票数目

	p = flightlink.head;
	while (p){														//查找航班并订票
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){		//查找到符合航班
			_Book_Tickets(hMainDlg, p, tickets_num);
			return TRUE;
		}
		p = p->Next;
	}//while
	MessageBox(hMainDlg, TEXT("对不起，该航班不存在，请确认您要订票的航班。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	return FALSE;
}//BookTickets()     


//订票
BOOL _Book_Tickets(HWND hwndDlg, Flight *p, int Tickets_Num){

	OrderForm *new_order = (OrderForm *)malloc(sizeof(OrderForm));
	if (!new_order){
		MessageBox(hwndDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (Tickets_Num > p->Vacant_Seat){					//余票不足，给出建议航班
		Recommend(hwndDlg, p, Tickets_Num);
		return FALSE;
	}

	new_order->Order_Number = ++allorder.AllOrderNum;	//新订单信息，总订单数+1
	new_order->Tickets_Num = Tickets_Num;
	lstrcpy(new_order->IdNum, passenger->IdNum);
	lstrcpy(new_order->Flight_Number, p->Flight_Number);
	lstrcpy(new_order->Departure, p->Departure);
	lstrcpy(new_order->Destination, p->Destination);
	lstrcpy(new_order->Date, p->Date);
	lstrcpy(new_order->TakeOff_Time, p->TakeOff_Time);
	lstrcpy(new_order->Landing_Time, p->Landing_Time);
	new_order->Next = NULL;
	new_order->psgNext = NULL;

	if (allorder.head == NULL)							//将结点添至订单链表末尾
		allorder.head = new_order;
	else
		allorder.tail->Next = new_order;
	allorder.tail = new_order;

	if (passenger->OrderLink.head == NULL)				//将结点添至用户订单链表末尾
		passenger->OrderLink.head = new_order;
	else
		passenger->OrderLink.tail->psgNext = new_order;
	passenger->OrderLink.tail = new_order;
	passenger->OrderLink.OrderNum++;
	passenger->TicketNum += Tickets_Num;

	p->Vacant_Seat -= Tickets_Num;						//航班余票减少

	MessageBox(hwndDlg, TEXT("订票成功！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);

	return TRUE;
}//_Book_Tickets()


//给出建议航班
BOOL Recommend(HWND hwndDlg, Flight *p, int Tickets_Num){

	SetDlgItemText(hwndDlg, IDC_INFORMATION, TEXT("\0"));	//清空航班信息栏

	int flag = 0;
	TCHAR szBuffer[256] = { TEXT("\0") };
	HWND hEdit = GetDlgItem(hwndDlg, IDC_INFORMATION);
	Flight *q;

	q = flightlink.head;
	while (q){

		if (!(lstrcmp(p->Departure, q->Departure)) && !(lstrcmp(p->Destination, q->Destination)) &&
			lstrcmp(p->Flight_Number, q->Flight_Number) && Tickets_Num <= q->Vacant_Seat)
		{
			if (!flag){
				wsprintf(szBuffer, TEXT("您所订航班余票不足，以下为建议航班:\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				wsprintf(szBuffer, TEXT("\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				wsprintf(szBuffer, TEXT("航班号\t出发地\t目的地\t起飞时间\t降落时间\t余座\t价格\t折扣\t出发日期\n"));
				SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
				SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
				++flag;
			}//if
			PrintFlight(hwndDlg, q);
		}//if
		q = q->Next;
	}//while

	if (!flag){
		wsprintf(szBuffer, TEXT("对不起，您所订航班余票不足，当天也其它没有符合条件的航班...\n"));
		SendMessage(hEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
		SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM)(szBuffer));
	}
	return TRUE;
}//Recommend()


//退票
BOOL ReturnTickets(HWND hwndDlg){
	int flag = 0;
	int num = 0, tickets_num;
	Flight *flight;
	OrderForm *p, *q, *s, *prePoint;
	TCHAR szFltNum[16], szDate[16];

	GetDlgItemText(hwndDlg, IDC_FLIGHTNUM, szFltNum, 15);		//获取用户输入航班号、航班日期
	GetDlgItemText(hwndDlg, IDC_DATE, szDate, 15);				

	if (!lstrlen(szFltNum)){									//未输入航班号
		MessageBox(hwndDlg, TEXT("请输入航班号！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	DialogBox(hInst, MAKEINTRESOURCE(IDD_FLTNUM), NULL, (DLGPROC)FlightNumDlgProc);	//获取退票数目

	if (!lstrlen(szBuffer))
		return FALSE;

	tickets_num = _ttoi(szBuffer);								//退票数

	p = passenger->OrderLink.head;
	prePoint = p;
	if (!p){													//用户无订单
		MessageBox(hwndDlg, TEXT("当前用户没有已订票"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	while (p){													//退票航班该用户订票总数
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate))
			num += p->Tickets_Num;
		p = p->psgNext;
	}
	if (tickets_num > num){										//退票数大于该航班已订票数
		wsprintf(szBuffer, TEXT("对不起，该航班您只订了%d张机票，请重新填写退票张数"), num);
		MessageBox(hwndDlg, szBuffer, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}//if

	p = passenger->OrderLink.head;
	while (p){													//退票
		if (!lstrcmp(p->Flight_Number, szFltNum) && !lstrcmp(p->Date, szDate)){
			if (tickets_num >= p->Tickets_Num){					//当退票数大于等于当前订单内票数

				s = p->psgNext;
				passenger->TicketNum -= p->Tickets_Num;			//用户订票数减少
				allorder.AllOrderNum--;							//总订单数减少
				passenger->OrderLink.OrderNum--;				//用户订单数减少
				tickets_num -= p->Tickets_Num;					//需要退票数减少  若需要退票数为0，则退出，否则继续循环

				if (passenger->OrderLink.head == p){			//若p为头结点，删除头结点
					passenger->OrderLink.head = p->psgNext;		//头指针指向下一个结点
					if (!p->psgNext)							//若p同时为尾结点，则尾指针指向NULL
						passenger->OrderLink.tail = NULL;
				}
				else if (!p->psgNext)							//若p非头结点但为尾结点，尾指针指向前一个结点
					passenger->OrderLink.tail = prePoint;
				prePoint->psgNext = p->psgNext;					//链表删除结点p

				flight = flightlink.head;
				while (flight){									//航班余座增加
					if (!lstrcmp(flight->Flight_Number, szFltNum) && !lstrcmp(flight->Date, szDate)){
						flight->Vacant_Seat += p->Tickets_Num;
						break;
					}
					flight = flight->Next;
				}//while

				q = allorder.head;
				prePoint = q;
				while (q){										//删除订单链表中订单结点
					if (q == p){								
						if (allorder.head == p){				//若p为头结点，删除头结点
							allorder.head = p->Next;			
							if (!q->Next)						//若q同时为尾结点
								allorder.tail = NULL;
						}
						else if (!q->Next)						//若q非头结点但为尾结点
							allorder.tail = prePoint;
						prePoint->Next = q->Next;				 
						free(p); break;							//释放p结点内存
					}//if
					prePoint = q;
					q = q->Next;
				}//while

				if (tickets_num){								//若退票数非0，则继续退票
					p = s; continue;
				}

				MessageBox(hwndDlg, TEXT("退票成功！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}//if
			else{												//退票数小于该订单订票数
				p->Tickets_Num -= tickets_num;					//订单订票数减少
				passenger->TicketNum -= tickets_num;			//用户订票数减少

				flight = flightlink.head;
				while (flight){
					if (!lstrcmp(flight->Flight_Number, szFltNum) && !lstrcmp(flight->Date, szDate)){
						flight->Vacant_Seat += tickets_num;		//航班余票增加
						break;
					}
					flight = flight->Next;
				}//while

				MessageBox(hwndDlg, TEXT("退票成功！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
				return TRUE;									//退票成功
			}//else
		}//if
		prePoint = p;
		p = p->psgNext;
	}//while

	wsprintf(szBuffer, TEXT("对不起，您没有订该航班机票，请确认退票航班"));
	MessageBox(hwndDlg, szBuffer, TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	return FALSE;
}//ReturnTickets()


//录入航班
BOOL EntryFlight(HWND hEntryDlg){

	TCHAR szDiscount[16];
	TCHAR szFare[16], szSeat_Number[16];
	TCHAR szFlight_Number[16], szDate[16];
	TCHAR szDeparture[16], szDestination[16];
	TCHAR szTakeOff_Time[16], szLanding_Time[16];

	Flight *p = (Flight *)malloc(sizeof(Flight)), *q;
	if (!p){
		MessageBox(hEntryDlg, TEXT("内存申请错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	GetDlgItemText(hEntryDlg, IDC_DEPARTURE, szDeparture, 16);			//获取录入航班信息
	GetDlgItemText(hEntryDlg, IDC_DESTINATION, szDestination, 16);
	GetDlgItemText(hEntryDlg, IDC_FLTNUM, szFlight_Number, 16);
	GetDlgItemText(hEntryDlg, IDC_FARE, szFare, 16);
	GetDlgItemText(hEntryDlg, IDC_TAKEOFFTIME, szTakeOff_Time, 16);
	GetDlgItemText(hEntryDlg, IDC_LANDINGTIME, szLanding_Time, 16);
	GetDlgItemText(hEntryDlg, IDC_SEATNUM, szSeat_Number, 16);
	GetDlgItemText(hEntryDlg, IDC_DISCOUNT, szDiscount, 16);
	GetDlgItemText(hEntryDlg, IDC_DATE, szDate, 16);

	if (!lstrlen(szDeparture) || !lstrlen(szDestination) || !lstrlen(szFlight_Number) || !lstrlen(szFare) ||
		!lstrlen(szTakeOff_Time) || !lstrlen(szLanding_Time) || !lstrlen(szSeat_Number))
	{
		MessageBox(hEntryDlg, TEXT("请您填写完整航班信息！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	q = flightlink.head;
	while (q){
		if (!lstrcmp(q->Flight_Number, szFlight_Number) && !lstrcmp(q->Date, szDate)){
			MessageBox(hEntryDlg, TEXT("对不起，同一航班在一天内只能有一班"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			free(p);
			return FALSE;
		}
		q = q->Next;
	}//while

	if (!lstrlen(szDiscount))
		wsprintf(szDiscount, "无");

	lstrcpy(p->Date, szDate);							//填入录入航班结点信息
	lstrcpy(p->Discount, szDiscount);
	lstrcpy(p->Departure, szDeparture);
	lstrcpy(p->Destination, szDestination);
	lstrcpy(p->TakeOff_Time, szTakeOff_Time);
	lstrcpy(p->Landing_Time, szLanding_Time);
	lstrcpy(p->Flight_Number, szFlight_Number);
	
	p->Fare = _ttof(szFare);
	p->Seat_Number = _ttoi(szSeat_Number);
	p->Vacant_Seat = p->Seat_Number;
	p->Next = NULL;

	if (flightlink.head == NULL)						//将新结点添至航班链表
		flightlink.head = p;
	else
		flightlink.tail->Next = p;
	flightlink.tail = p;
	flightlink.Flight_Number++;							//航班数目增加

	MessageBox(hEntryDlg, TEXT("航班录入成功！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);

	return TRUE;
}//EntryFlight()


//修改航班信息
BOOL ModifyFlight(HWND hModifyDlg){
	int seat_num;
	TCHAR szDiscount[16];
	TCHAR szFare[16], szSeat_Number[16];
	TCHAR szTakeOff_Time[16], szLanding_Time[16];
	TCHAR Mdf_flt_num[16], Mdf_flt_date[16];

	OrderForm *q;
	Flight *p;

	GetDlgItemText(hModifyDlg, IDC_MDFFLTNUM, Mdf_flt_num, 16);				//获取要修改的航班号、航班日期
	GetDlgItemText(hModifyDlg, IDC_MDFDATE, Mdf_flt_date, 16);

	if (!lstrlen(Mdf_flt_num)){
		MessageBox(hModifyDlg, TEXT("请输入所要修改信息的航班号"), TEXT("信息"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	GetDlgItemText(hModifyDlg, IDC_FARE, szFare, 16);						//获取要修改的信息
	GetDlgItemText(hModifyDlg, IDC_DISCOUNT, szDiscount, 16);
	GetDlgItemText(hModifyDlg, IDC_SEATNUM, szSeat_Number, 16);
	GetDlgItemText(hModifyDlg, IDC_TAKEOFFTIME, szTakeOff_Time, 16);
	GetDlgItemText(hModifyDlg, IDC_LANDINGTIME, szLanding_Time, 16);

	p = flightlink.head;
	while (p){																//查找要修改的航班
		if (!lstrcmp(Mdf_flt_num, p->Flight_Number) && !lstrcmp(Mdf_flt_date, p->Date)){
			if (lstrlen(szFare))											//改变航班票价
				p->Fare = _ttof(szFare);
			if (lstrlen(szDiscount))										//改变航班折扣
				wsprintf(p->Discount, szDiscount);
			if (lstrlen(szSeat_Number)){									//改变航班座位数
				seat_num = _ttoi(szSeat_Number);
				if ((p->Seat_Number - seat_num) > p->Vacant_Seat){			//余座数量>=0
					MessageBox(hModifyDlg, TEXT("对不起，座位数过少，请重新填写"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
					return FALSE;
				}
				p->Vacant_Seat -= p->Seat_Number - seat_num;				//改变余票数量
				p->Seat_Number = seat_num;									//改变座位数
			}//if
			if (lstrlen(szTakeOff_Time)){									//改变航班起飞时间
				wsprintf(p->TakeOff_Time, szTakeOff_Time);
				q = allorder.head;											
				while (q){													//改变该航班所有订单出发时间
					if (!lstrcmp(q->Flight_Number, p->Flight_Number) && !lstrcmp(q->Date, p->Date))
						wsprintf(q->TakeOff_Time, szTakeOff_Time);
					q = q->Next;
				}
			}//if
			if (lstrlen(szLanding_Time)){									//改变航班降落时间
				wsprintf(p->Landing_Time, szLanding_Time);
				q = allorder.head;											
				while (q) {													//改变该航班所有订单降落时间
					if (!lstrcmp(q->Flight_Number, p->Flight_Number) && !lstrcmp(q->Date, p->Date))
						wsprintf(q->Landing_Time, szLanding_Time);
					q = q->Next;
				}
			}//if
			MessageBox(hModifyDlg, TEXT("修改成功！"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
			return TRUE;
		}//if
		p = p->Next;
	}//while

	MessageBox(hModifyDlg, TEXT("对不起，您所要修改的航班尚未录入。"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
	return FALSE;

}//ModifyFlight()


//保存账户资料
BOOL WriteAccountData(HWND hMainDlg){
	Passenger *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\AccountData.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("账户信息保存出现错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	p = psglink.head;
	while (p){
		fprintf(fp, "%s\n%s\n%s\n", p->Name, p->IdNum, p->PassWord);
		q = p;
		p = p->Next;
		free(q);
	}//while

	fclose(fp);
	return TRUE;
}//WriteAccountData


//保存订单信息
BOOL WriteOrderData(HWND hMainDlg){
	OrderForm *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\OrderForm.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("订单信息保存出现错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	p = allorder.head;
	while (p){
		fprintf(fp, "%s\n", p->IdNum);
		fprintf(fp, "%d\n", p->Order_Number);
		fprintf(fp, "%d\n", p->Tickets_Num);
		fprintf(fp, "%s\n", p->Flight_Number);
		fprintf(fp, "%s\n", p->Departure);
		fprintf(fp, "%s\n", p->Destination);
		fprintf(fp, "%s\n", p->Date);
		fprintf(fp, "%s\n", p->TakeOff_Time);
		fprintf(fp, "%s\n", p->Landing_Time);
		q = p;
		p = p->Next;
		free(q);
	}//while
	fclose(fp);
	return TRUE;
}//WriteOrderData()


//保存航班信息
BOOL WriteFlightData(HWND hMainDlg){
	Flight *p,*q;
	FILE *fp;
	if ((fp = fopen(".\\FlightData.txt", "w")) == NULL){
		MessageBox(hMainDlg, TEXT("航班信息保存出现错误!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	p = flightlink.head;
	while (p){
		fprintf(fp, "%s\n", p->Flight_Number);
		fprintf(fp, "%.2lf\n", p->Fare);
		fprintf(fp, "%s\n", p->Discount);
		fprintf(fp, "%d\n%d\n", p->Seat_Number, p->Vacant_Seat);
		fprintf(fp, "%s\n", p->Departure);
		fprintf(fp, "%s\n", p->Destination);
		fprintf(fp, "%s\n", p->Date);
		fprintf(fp, "%s\n", p->TakeOff_Time);
		fprintf(fp, "%s\n", p->Landing_Time);
		q = p;
		p = p->Next;
		free(q);
	}//while
	fclose(fp);
	return TRUE;
}//WriteFlight()