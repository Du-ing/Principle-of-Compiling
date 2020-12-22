#include "thompson.h"

int STATE_NUM = 0;

/**表达式转NFA处理函数,返回最终的NFA结合
*/
cell express_2_NFA(string expression)
{
	int length = expression.size();
	char element;
	cell Cell, Left, Right;
	stack<cell> STACK;
	for (int i = 0; i < length; i++)
	{
		element = expression.at(i);
		switch (element)
		{
		case '|':
			Right = STACK.top();
			STACK.pop();
			Left = STACK.top();
			STACK.pop();
			Cell = do_Unite(Left, Right);
			STACK.push(Cell);
			break;
		case '*':
			Left = STACK.top();
			STACK.pop();
			Cell = do_Star(Left);
			STACK.push(Cell);
			break;
		case '+':
			Right = STACK.top();
			STACK.pop();
			Left = STACK.top();
			STACK.pop();
			Cell = do_Join(Left, Right);
			STACK.push(Cell);
			break;
		default:
			Cell = do_Cell(element);
			STACK.push(Cell);
		}
	}
	//cout << "处理完毕!" << endl;
	Cell = STACK.top();
	STACK.pop();

	return Cell;
}

//处理 a|b
cell do_Unite(cell Left, cell Right)
{
	edge Edge1;

	for (int i = 0; i < Right.EdgeCount; i++) {
		Edge1 = Right.EdgeSet[i];
		Edge1.StartState.StateName = Left.StartState.StateName;
		Edge1.EndState.StateName = Left.EndState.StateName;
		STATE_NUM -= 2;
		Left.EdgeSet[Left.EdgeCount++] = Edge1;
	}
	return Left;
}
//处理 ab
cell do_Join(cell Left, cell Right)
{
	//将left中的结束状态和right中的开始状态合并
	for (int i = 0; i < Right.EdgeCount; i++)
	{
		if (Right.EdgeSet[i].StartState.StateName==Right.StartState.StateName)
		{	
			for (int j = 0; j < Right.EdgeCount; j++) {
				Right.EdgeSet[j].StartState.StateName--;
				Right.EdgeSet[j].EndState.StateName--;
			}
			Right.StartState.StateName--;
			Right.EndState.StateName--;
			//Right.EdgeSet[i].StartState = Left.EndState;
			//Right.EdgeSet[i].EndState.StateName--;
			Left.EdgeSet[Left.EdgeCount++] = Right.EdgeSet[i];
			STATE_NUM--;
		}
		
		
	}
	for (int i = 0; i < Right.EdgeCount; i++)
	{	
		int flag = 1;
		for (int j = 0; j < Left.EdgeCount; j++) {
			if (Right.EdgeSet[i].StartState.StateName == Left.EdgeSet[j].StartState.StateName
				&&Right.EdgeSet[i].EndState.StateName == Left.EdgeSet[j].EndState.StateName
				&&Right.EdgeSet[i].TransSymbol==Left.EdgeSet[j].TransSymbol)
				flag = 0;
		}
		if(flag)Left.EdgeSet[Left.EdgeCount++] = Right.EdgeSet[i];
		

	}
	//Right.EndState.StateName--;//因为上面改不会改变这里，所以这里需要再改一次
	Left.EndState = Right.EndState;
	return Left;
}
//处理 a*
cell do_Star(cell Cell)
{
	
	edge Edge1, Edge2, Edge3;
	state newState = new_StateNode();
	state left = Cell.StartState;
	state right = Cell.EndState;
	for (int i = 0; i < Cell.EdgeCount; i++) {//将以开头为起点的所有起始边转到结尾
		if (Cell.EdgeSet[i].StartState.StateName == Cell.StartState.StateName) {
			Cell.EdgeSet[i].StartState.StateName = right.StateName;
		}
	}
	//加入左边的伊布斯隆
	Edge2.StartState.StateName = left.StateName;
	Edge2.EndState.StateName = right.StateName;
	Edge2.TransSymbol = '~';
	Cell.EdgeSet[Cell.EdgeCount++] = Edge2;
	//加入右边的伊布斯隆
	Edge3.StartState.StateName = right.StateName;
	Edge3.EndState.StateName = newState.StateName;
	Edge3.TransSymbol = '~';
	Cell.EdgeSet[Cell.EdgeCount++] = Edge3;
	//更新endsstate
	Cell.EndState = Edge3.EndState;
	
	return Cell;
}
//处理 a
cell do_Cell(char element)
{
	cell NewCell;
	NewCell.EdgeCount = 0;
	edge NewEdge;
	//获得新的新状态节点
	state StartState = new_StateNode();
	state EndState = new_StateNode();
	//构建边
	NewEdge.StartState = StartState;
	NewEdge.EndState = EndState;
	NewEdge.TransSymbol = element;
	//构建单元
	NewCell.EdgeSet[NewCell.EdgeCount++] = NewEdge;
	NewCell.StartState = NewCell.EdgeSet[0].StartState;
	NewCell.EndState = NewCell.EdgeSet[0].EndState;//EdgeCount此时为1
	return NewCell;
}
void cell_EdgeSet_Copy(cell& Destination, cell Source)
{
	int D_count = Destination.EdgeCount;
	int S_count = Source.EdgeCount;
	for (int i = 0; i < S_count; i++)
	{
		Destination.EdgeSet[D_count + i] = Source.EdgeSet[i];
	}
	Destination.EdgeCount = D_count + S_count;
}
/*
获得新的状态节点，统一产生，便于管理，不能产生重复的状态
并添加到state_set[]数组中
*/
state new_StateNode()
{
	state newState;
	newState.StateName = STATE_NUM ;
	STATE_NUM++;
	return newState;
}
//接收输入正规表达式，RegularExpression作为回传函数
void input(string &RegularExpression)
{
	
		cin >> RegularExpression;


}
/**检测输入的正则表达式是否合法
*/
int check_legal(string check_string)
{
	if (check_character(check_string) && check_parenthesis(check_string))
	{
		return true;
	}
	return false;
}
/**
检查输入的字符是否合适 () * | a~z A~Z
合法返回true,非法返回false
*/
int check_character(string check_string)
{
	int length = check_string.size();
	for (int i = 0; i < length; i++)
	{
		char check = check_string.at(i);
		if (is_letter(check))//小写和大写之间有5个字符，故不能连续判断
		{
			//cout<<"字母 合法";
		}
		else if (check == '(' || check == ')' || check == '*' || check == '|')
		{
			//cout<<"操作符 合法";
		}
		else
		{
			//cout << "含有不合法的字符!" << endl;
			//cout << "请重新输入:" << endl;
			return false;
		}
	}
	return true;
}
/**先检查括号是否匹配
*合法返回true,非法返回false
*/
int check_parenthesis(string check_string)
{
	int length = check_string.size();
	//char * check = new char[length+1];
	//wcscpy(check, length+1, check_string.c_str());
	string check = check_string;
	stack<int> STACK;
	for (int i = 0; i < length; i++)
	{
		if (check[i] == '(')
			STACK.push(i);
		else if (check[i] == ')')
		{
			if (STACK.empty())
			{
			//	cerr << "有多余的右括号" << endl;//暂时不记录匹配位置location
			//	cout << "请重新输入:" << endl;
				return false;
			}
			else
				STACK.pop();
		}
	}
	if (!STACK.empty())
	{
		//暂时不记录匹配位置location
	//	cerr << "有多余的左括号" << endl;
	//	cout << "请重新输入:" << endl;
		return false;
	}

	return true;
}
/**检测是否是字母
是返回true,否则false
*/
int is_letter(char check)
{
	if (check >= 'a'&&check <= 'z' || check >= 'A'&&check <= 'Z'||(check>='0'&&check<='9'))
		return true;
	return false;
}

/**添加交操作符“+”，便于中缀转后缀表达式
例如 abb->a+b+b
*/
string add_join_symbol(string add_string)
{
	int length = add_string.size();
	int return_string_length = 0;
	char *return_string = new char[2 * length + 2];
	char first, second;
	for (int i = 0; i < length - 1; i++)
	{
		first = add_string.at(i);
		second = add_string.at(i + 1);
		return_string[return_string_length++] = first;
		//要加的可能性如ab 、 *b 、 a( 、 )b 等情况
		//若第二个是字母、第一个不是'('、'|'都要添加
		if (first != '('&&first != '|'&&is_letter(second))
		{
			return_string[return_string_length++] = '+';
		}
		//若第二个是'(',第一个不是'|'、'(',也要加
		else if (second == '('&&first != '|'&&first != '(')
		{
			return_string[return_string_length++] = '+';
		}
	}
	//将最后一个字符写入
	return_string[return_string_length++] = second;
	return_string[return_string_length] = '\0';
	string STRING(return_string);
	//cout << "加'+'后的表达式：" << STRING << endl;
	return STRING;
}
/*
构造优先级表规则：（1）先括号内，再括号外；（2）优先级由高到低：闭包、|、+；（3）同级别，先左后右。
优先级表：
	 #	(	*	|	+	)
isp  0	1	7	5	3	8
icp	 0	8	6	4	2	1
*/

/*********************运算符优先级关系表********************/
/*
	c1\c2	(	*	|	+	)	#

	(		<	<	<	<	=	>

	*		<	>	>	>	>	>

	|		<	<	>	>	>	>

	+		<	<	<	>	>	>

	#		<	<	<	<	<	=
*/
/***********************************************************/

//in stack priority  栈内优先级，栈顶的字符的优先级
int isp(char c)
{
	switch (c)
	{
	case '#': return 0;
	case '(': return 1;
	case '*': return 7;
	case '|': return 5;
	case '+': return 3;
	case ')': return 8;
	}
	//若走到这一步，说明出错了
	//cerr << "ERROR!" << endl;
	return false;
}
//in coming priority 栈外优先级，当前扫描到的字符的优先级
int icp(char c)
{
	switch (c)
	{
	case '#': return 0;
	case '(': return 8;
	case '*': return 6;
	case '|': return 4;
	case '+': return 2;
	case ')': return 1;
	}
	//若走到这一步，说明出错了
	//cerr << "ERROR!" << endl;
	return false;
}
/**中缀表达式转后缀表达式
*/
string postfix(string e)
{
	//设定e的最后一个符号式“#”，而其“#”一开始先放在栈s的栈底
	e = e + "#";

	stack<char> s;
	char ch = '#', ch1, op;
	s.push(ch);
	//读一个字符
	string out_string = "";
	int read_location = 0;
	ch = e.at(read_location++);
	while (!s.empty())
	{
		if (is_letter(ch))
		{
			out_string = out_string + ch;
			ch = e.at(read_location++);
		}
		else
		{
			ch1 = s.top();
			if (isp(ch1) < icp(ch))
			{
				s.push(ch);
				ch = e.at(read_location++);
			}
			else if (isp(ch1) > icp(ch))//
			{
				op = s.top();
				s.pop();
				out_string = out_string + op;
			}
			else  //考虑优先级相等的情况
			{
				op = s.top();
				s.pop();
				if (op == '(')
					ch = e.at(read_location++);
			}
		}
	}

	//cout << "后缀表达式：" << out_string << endl;
	return out_string;
}
/*
	显示NFA
*/

void Display(cell Cell)
{
	ofstream outFile;
	outFile.open("outFile1.txt");

	int begin = Cell.StartState.StateName;
	int end = Cell.EndState.StateName;
	int cnt = Cell.EdgeCount;

	cout << "X ";
	outFile << "X ";
	for (int j = 0; j < cnt; j++)
	{
		if (Cell.EdgeSet[j].StartState.StateName == 0)
		{
			if (Cell.EdgeSet[j].EndState.StateName == end)
			{
				cout << "X-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
				outFile << "X-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
			}
			else
			{
				cout << "X-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName-1 << " ";
				outFile << "X-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName-1 << " ";
			}
		}
	}
	cout << endl << "Y" << endl;
	outFile << endl << "Y" << endl;



	for (int i = begin+1; i < end; i++)
	{
		int flag = 1;
		for (int j = 0; j < cnt; j++)
		{
			if (Cell.EdgeSet[j].StartState.StateName==i&&Cell.EdgeSet[j].TransSymbol == '~')
			{
				if (flag)
				{
					cout << i - 1 << " ";
					outFile << i - 1 << " ";
					flag = 0;
				}
				if (Cell.EdgeSet[j].EndState.StateName == end)
				{
					cout << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
					outFile << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
				}
				else
				{
					cout << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName - 1 << " ";
					outFile << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName - 1 << " ";
				}
				Cell.EdgeSet[j].StartState.StateName = 2342423;
			}
		}
		for (int j = 0; j < cnt; j++)
		{
			if (Cell.EdgeSet[j].StartState.StateName == i)
			{
				if (flag)
				{
					cout << i-1 << " ";
					outFile << i-1 << " ";
					flag = 0;
				}
				if(Cell.EdgeSet[j].EndState.StateName==end)
				{
					cout << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
					outFile << i - 1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << "Y ";
				}
				else
				{
					cout << i-1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName-1 << " ";
					outFile << i-1 << "-" << Cell.EdgeSet[j].TransSymbol << "->" << Cell.EdgeSet[j].EndState.StateName-1 << " ";
				}
			}
		}
		if (!flag)
		{
			cout << endl;
			outFile << endl;
		}
	}
	outFile.close();
}

int p1()
{
	string Regular_Expression;
	cell NFA_Cell;
	Regular_Expression = "(a|b)*abb";
	//接受输入
	// input(Regular_Expression);
	//添加“+”，便于转后缀表达式
	Regular_Expression = add_join_symbol(Regular_Expression);
	//中缀转后缀
	Regular_Expression = postfix(Regular_Expression);
	//表达式转NFA
	NFA_Cell = express_2_NFA(Regular_Expression);
	//显示
	
	Display(NFA_Cell);
	return 0;
}