/* 
    * 此文件用于实现一些基本操作所依赖的底层
*/

struct Columns *LoadFile (char *filename);      /*本函数用于对文件的读取*/
struct Columns *ColHead (struct Columns *now_head, struct Rows *now_rows_head, int col_position, int window_x, int window_y);       /*此函数可由当前行的位置算出行头*/
struct Rows *ColHeadRows (struct Columns *now_head, struct Rows *now_head_rows, int col_position, int window_x, int window_y);      /* 此函数可由当前行的位置算出该行的行头元素*

/*
    * 本函数用于对文件的读取
    * 传入参数：filename，将一个文件名传入，可包含地址
    * 返回值：　返回形成的二维链表的行的头文件
    * 形成的二维链表，行链表包括第０行zero和最后一行tail，其余行均在此两行之间
    * 形成的二维链表，列链表都有一个表头，此表头不存数据
*/
struct Columns *LoadFile (char * filename)
{
    FILE *file;

    int ch;
    int columns = 0;        //用于统计每行有多少个元素
    int count = 0;
    file = fopen(filename, "rw+");

    struct Columns * p;
    struct Columns * fist;      //所有行的头指针，也就是第一行
    struct Columns * columns_front;
    struct Rows * rows_front;

    struct Columns * zero = (struct Columns *)malloc(sizeof(struct Columns));       //第0行
    zero->head = (struct Rows *)malloc(sizeof(struct Rows));
    zero->next = NULL;
    zero->prior = NULL;
    zero->rowscount = 0;
    zero->head->next = NULL;
    zero->head->prior = NULL;

    /*初始化，当读入文件为空时*/
    if(file == NULL)
    {
        /*读取文件为空时的行头*/
        struct Columns * nofile = (struct Columns *)malloc(sizeof(struct Columns));
        nofile->head = (struct Rows *)malloc(sizeof(struct Rows));
        nofile->head->next = NULL;
        nofile->head->prior = NULL;

        nofile->next = NULL;
        nofile->prior = zero;
        nofile->rowscount = 0;
        zero->next = nofile;

        /*给链表最后行弄一条尾巴*/
        struct Columns * tail = (struct Columns *)malloc(sizeof(struct Columns));
        struct Rows * tail_rows = (struct Rows *)malloc(sizeof(struct Rows));
        tail_rows->prior = NULL;
        tail_rows->next = NULL;
        tail->head = tail_rows;
        tail->rowscount = 0;
        tail->next = NULL;
        tail->prior = nofile;
        nofile->next = tail;

        return nofile;
    }

    /*若读入的文件不为空*/
    columns_front = zero;
    while ((ch = fgetc(file))!= EOF)
    {
        p = (struct Columns *)malloc(sizeof(struct Columns));   /*分配一行*/
        p->head = (struct Rows *)malloc(sizeof(struct Rows));   /*在行内分配一列, 该列为第0列，即每列的列头*/
        p->head->next = NULL;
        p->head->prior = NULL;

        rows_front = p->head;   //对于每一列行的头节点，都要初始化给一个front

        /*每一行直到换行前，不断尾插，每一行形成一条链表*/
        while(ch != '\n')
        {
            struct Rows * elem = (struct Rows *)malloc(sizeof(struct Rows));    /*数据节点，用于保存字符*/
            elem->Elemment = ch;
            elem->next = NULL;

            elem->prior = rows_front;
            rows_front->next = elem;
            rows_front = elem;
            columns++;

            ch = fgetc(file);   /*刷新缓冲*/
        }

        count++;

        if(count == 1) fist = p;        /*设置第一行为形成二维链表的行头指针*/
        p->next = NULL;
        p->rowscount = columns;
        columns_front->next = p;
        p->prior = columns_front;
        columns_front = p;

        columns = 0;
    }

    //给链表最后行弄一条尾巴，即行尾
    struct Columns * tail = (struct Columns *)malloc(sizeof(struct Columns));
    struct Rows * tail_rows = (struct Rows *)malloc(sizeof(struct Rows));
    tail_rows->prior = NULL;
    tail_rows->next = NULL;
    tail->head = tail_rows;
    tail->rowscount = 0;
    tail->next = NULL;
    tail->prior = p;
    p->next = tail;

    fclose(file);
    return fist;        /*返回行头*/
}


/*
    * 此函数可由当前行的位置算出该行的行头
    * 传入参数：
    * now_head：当前显示在窗口的第一行
    * now_head：当前显示在窗口的第一行的第一列，即该行的第一个元素
    * col_position：要计算行的位置
    * window_x：窗口行大小
    * window_y：窗口列大小
    * 返回值：返回该行的行头
*/
struct Columns *ColHead (struct Columns *now_head, struct Rows *now_rows_head, int col_position, int window_x, int window_y)      //由行算出该行头
{
   struct Columns * colhead = now_head;
   struct Columns * p = now_head;
   struct Rows * rows = now_rows_head;

   for(int i = 1; i <= window_y - 2 && p != NULL; i++)
	{
		for(int j = 1; j <= window_x - 2 && rows->next != NULL;j++)
		{
			rows = rows->next;
		}
        if(col_position == i) return colhead;
        if(rows->next == NULL) 
        {
            p = p->next; 
            colhead = p; 
            rows = p->head;
        }
	}
}


/*
    * 此函数可由当前行的位置算出该行的行头元素
    * 传入参数：
    * now_head：当前显示在窗口的第一行
    * now_head：当前显示在窗口的第一行的第一列，即该行的第一个元素
    * col_position：要计算行的位置
    * window_x：窗口行大小
    * window_y：窗口列大小
    * 返回值：返回该行的行头元素
*/
struct Rows *ColHeadRows (struct Columns *now_head, struct Rows *now_head_rows, int col_position, int window_x, int window_y)      
{
    /*定义一些变量*/
    struct Columns *position_find = now_head;
    struct Rows *returnhead;
    struct Rows *rows = now_head_rows;
    int count = 1;

    if(col_position == 1) return rows;

    for(int i = 1; i <= window_y - 2 && position_find != NULL; i++)
	{
		for(int j = 1; j <= window_x - 2 && rows->next != NULL;j++)
		{
			    rows = rows->next;
        }
        count++;
        if(rows->next == NULL) 
        { 
            position_find = position_find->next; rows = position_find->head; returnhead = rows; 
        }
        else 
            returnhead = rows;
        if(col_position == count) return returnhead;
	}
}

