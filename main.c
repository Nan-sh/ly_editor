/*
    * 项目名称：　基于c语言和ncurses的文本编辑器
    * 创建人：  南树，2020年8月12日-----
    * 思想：　用一个二维双链表作为底层数据结构，分别表示文本的行和列，将文本的每个字符存为一个结点，基于这个数据结构完成对文本的增删改查
*/

/*头文件*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

/*数据结构的定义*/
struct Rows {
    int Elemment;
    struct Rows * next;
    struct Rows * prior;
};

struct Columns {
    struct Rows * head;
    int rowscount;
    struct Columns * next;
    struct Columns * prior;
};

/*主程序*/
int main()
{
    /*读取文件*/
	char *filename = "main.c";
	struct Columns *get_data_columns_head = Load(filename);
    struct Rows *get_data_rows_head = get_data_columns_head->head;

    /*显示窗口的定义*/
    WINDOW * my_win;    /*新建窗口*/
    initscr();
	cbreak();
	nonl();
	noecho();
	intrflush(win, false);
	refresh();
    setlocale(LC_ALL,"");
	initial(my_win);
    keypad(my_win, true);

	int x = 1;
	int y = 1;      /*窗口坐标初始化*/

    int window_size_x = getmaxx(stdscr);
    int window_size_y = getmaxy(stdscr);     /*窗口大小的初始化*/
    
    /*绘制窗口*/
	my_win = newwin(window_size_x, window_size_y, 0, 0);
	box(my_win, '|', '-');
	show(my_win, get_data_columns_head, get_data_rows_head, &x, &y, window_size_x, window_size_y);
    

    /*循环，用于实现各种操作*/
	do
	{
		int ch = wgetch(my_win);

		switch(ch)
		{
			case KEY_UP:
                if(y > 1)
                    Up_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);

                else if(p_move->prior->prior != NULL)
                {
                    Up_Up(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    if(ColLine(p_move_rows) <= WINDOW_X - 2)
                    {
                        x = ColLine(p_move_rows);
                        if(x == 0) x = 1;
                    }
                    else
                        x = (WINDOW_X - 2) / 2;
                    y = 1;

                }
                break;
			case KEY_DOWN:
                if(ColHead(p_move, p_move_rows, y, WINDOW_X, WINDOW_Y)->next != NULL)
                    Down_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                if(y > 30)
                {
                    Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                }
				break;
			case KEY_RIGHT:
				RightFunction(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                break;
			case KEY_LEFT:
                LeftFunction(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                break;

                int tmp_enter_x;
                int tmp_enter_y;
            case '\r':
                tmp_enter_x = x;
                tmp_enter_y = y;

                if(tmp_enter_x == 1 && tmp_enter_y == 1)        //若在第一行前插入，则先把显示区上移增加后再下移
                {
                    int count = p_move->rowscount / (WINDOW_X - 2);
                    for(int i = 0; i <= count; i++)
                    {
                        Up_Up(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                        x = tmp_enter_x; y = i + 2;
                    }

                    Enter_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    for(int i = 0; i <= count; i++)
                    {
                        Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    }
                    wclear(my_win);
                    box(my_win, '|', '-');
                    show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    x = 1;
                    y = tmp_enter_y + 1;
                    break;
                }

                Enter_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                wclear(my_win);
                box(my_win, '|', '-');
                show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                x = 1;
                y = tmp_enter_y + 1;

                if(tmp_enter_y == WINDOW_Y - 2)     //若显示区最后一行插入
                {
                    x = tmp_enter_x;
                    y = tmp_enter_y;
                    Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    y = tmp_enter_y;
                    x = 1;
                }
                break;

                int tmp_back_x;
                int tmp_back_y;
                int tmp_back_last;
            case KEY_BACKSPACE:
                tmp_back_x = x;
                tmp_back_y = y;
                if(tmp_back_y > 1) tmp_back_last = ColLine(ColHeadRows(p_move, p_move_rows, tmp_back_y - 1, WINDOW_X, WINDOW_Y));


                if(tmp_back_x == 1 && tmp_back_y == 1 && p_move_rows->prior != NULL)
                {
                    if(p_move->prior->prior == NULL)
                    {
                        x = 1;
                        y = 1;
                        break;
                    }
                    int count = p_move->rowscount / (WINDOW_X - 2);
                    for(int i = 0; i <= count; i++)
                    {
                        Up_Up(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                        x = tmp_enter_x; y = i + 2;
                    }
                    Backspace_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    for(int i = 0; i <= count - 1; i++)
                    {
                        Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    }
                    wclear(my_win);
                    box(my_win, '|', '-');
                    show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);

                    if(ColHeadRows(p_move, p_move_rows, 2, WINDOW_X, WINDOW_Y) != NULL)
                    {
                        x = WINDOW_X - 2;
                        y = 1;
                    }

                    break;
                }

                else{
                    if(p_move->prior->prior == NULL && tmp_back_x == 1 && tmp_back_y == 1)
                    {
                        x = 1;
                        y = 1;
                        break;
                    }

                    Backspace_Function(p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                    wclear(my_win);
                    box(my_win, '|', '-');
                    show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);

                    if(tmp_back_x > 1)      //如果当前坐标x>1
                    {
                        x = tmp_back_x - 1;
                        y = tmp_back_y;
                    }
                    else if(ColHeadRows(p_move, p_move_rows, tmp_back_y, WINDOW_X, WINDOW_Y)->prior == NULL)       //如果当前坐标x=1且本行与上一行不连接
                    {
                        x = tmp_back_last + 1;
                        y = tmp_back_y - 1;
                    }
                    else        //若当前坐标x＝１且与上一行连接
                    {
                        x = WINDOW_X - 2;
                        y = tmp_back_y - 1;
                    }
                }
                break;

                int tmp_default_x;
                int tmp_default_y;
                int flag = 0;
            default:
                //默认为输入
                tmp_default_x = x;
                tmp_default_y = y;
                
                
                Insert(p_move, p_move_rows, x, y, ch, WINDOW_X, WINDOW_Y);
                wclear(my_win);
                box(my_win, '|', '-');
                show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);
                x = tmp_default_x + 1;
                y = tmp_default_y;
                if(x > WINDOW_X - 2)
                {
                    x = WINDOW_X - 2;
                    flag = 1;
                }


                /*if(x == WINDOW_X - 1 && y != WINDOW_Y - 2) 
                {
                    struct Rows *tmp = ColNowRows(ColHeadRows(p_move, p_move_rows, tmp_default_y, WINDOW_X, WINDOW_Y), tmp_default_x);
                    if(tmp->next != NULL) ;
                    else
                    {struct Rows *rows_tail = (struct Rows *)malloc(sizeof(struct Rows));
                    
                    rows_tail->Elemment = ch;
                    rows_tail->next = NULL;
                    rows_tail->prior = tmp;
                    tmp->next = rows_tail;
                    ColHead(p_move, p_move_rows, y, WINDOW_X, WINDOW_Y)->rowscount++;

                    wclear(my_win);
                    box(my_win, '|', '-');
                    show(my_win, p_move, p_move_rows, &x, &y, WINDOW_X, WINDOW_Y);        //还有这里的问题，诶有点难啊
                    }

                    x = 1; y = tmp_default_y + 1;
                }*/


                //else if(x == WINDOW_X - 1 && y == WINDOW_Y - 2) { Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y); x = 1; }
                //else if(y == WINDOW_Y - 2 && ColLine(ColHeadRows(p_move, p_move_rows, WINDOW_Y - 2, WINDOW_X, WINDOW_Y)) > WINDOW_X - 2) { Down_Down(my_win, &p_move, &p_move_rows, &x, &y, WINDOW_X, WINDOW_Y); x = tmp_default_x + 1; y--; }

                break;
		}

        /*if(x < 1) x = 1;
        if(x > WINDOW_X - 2) x = WINDOW_X - 2;
        if(y < 1) y = 1;
        if(y > WINDOW_Y - 2) y = WINDOW_Y - 2;*/

		mvwprintw(my_win, y, x, "");
		wrefresh(my_win);

	}while(1);

	endwin();

    return 0;
}