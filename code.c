#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct ARRAY {                //structure for entering the table and working with it
    char*** rows;                     //variable for storing cells
    int** cells;                      //variable for the number of cells in a row
    int count;                        //variable for the number of rows in the table
} array;
typedef struct COMMAND {              //structure for entering orders
    char command[1000][1001];         //array for storing orders
    int count;                        //variable for counting the number of orders
} command;
typedef struct SELECTION {            //structure for selecting cells on which to work
    int row_min;                      //minimum line
    int row_max;                      //maximum line
    int col_min;                      //minimum column
    int col_max;                      //maximum column
} selection;
typedef struct TMP_CELLS {            //structure for temporary variables
    char* cells[10];                  //cell
    bool use[10];                     //a variable that indicates whether they were used or not
} tmp_cells;
char* tmp_ctor (char*, int*);                            //temporary variable memory allocation
char* tmp_realloc (char*, int*);                         //changing allocated memory to a temporary variable
void tmp_dtor (char*, int*);                             //freeing memory of a temporary variable
void fdelim_and_row (char*, char*);                      //dividing a row into cells(if you entered -d)
void fdelim_zero (char*, char);                          //dividing a row into cells(if you not entered -d)
void ctor_row (array*);                                  //memory allocation line
void ctor_cell (array*, int);                            //cell memory allocation
void ctor_length (array*, int, int, int);                //word memory allocation
void realloc_length (array*, int, int, int);             //change allocated word memory
void ctor_cells (array*);                                //memory allocation for the number of cells in a row
void realloc_count_cells (array*);                       //changing allocated memory for the number of cells in a row
void realloc_row (array*, int);                          //changing the memory allocated to a row
void realloc_cell (array*, int, int);                    //changing allocated memory for cells
void realloc_cells (array*, int, int);                  //change allocated memory for information of the number of cells
void row_transfer (array*, char*);                       //entering a string into cells
void free_arr (array*);                                  //freeing memory in a table
void free_tmp_cell (tmp_cells*);                         //freeing memory in temporary locations
void table_alignment (array*);                           //table alignment
void change_selection (char*, selection*, array*, tmp_cells*);//cell selection
selection max (selection, array);             //finds a cell with the maximum numeric value and sets the selection to it
selection min (selection, array);             //finds a cell with the minimum numeric value and sets the selection to it
selection find (selection, array, char*, tmp_cells*);    //selects the first cell whose value contains the substring STR
void irow (array*, selection);                           //inserts one blank row above the selected cells
void arow (array*, selection);                           //adds one blank row below the selected cells
void drow (array*, selection);                           //deletes the selected rows
void icol (array*, selection);                           //inserts one empty column to the left of the selected cells
void acol (array*, selection);                           //adds one empty column to the right of the selected cells
void dcol (array*, selection);                           //deletes the selected columns
void set (array*, selection, char*);                     //sets the cell value to the string STR
void clear (array*, selection);                          //the contents of the selected cells will be deleted
void swap (array*, selection, char*, tmp_cells*);        //replaces the contents of the selected cell
void sum (array*, selection, char*, tmp_cells*);         //stores the sum of the values of the selected cells
void avg (array*, selection, char*, tmp_cells*);         //same as sum, but the arithmetic mean is stored
void count (array*, selection, char*, tmp_cells*);       //same as sum, but the number of non-empty cells is stored
void flen (array*, selection, char*, tmp_cells*);        //stores the string length of the currently selected cell
void fdef (array*, selection, char*, tmp_cells*);   //the value of the current cell will be set to a temporary variable
void fuse (array*, selection, char*, tmp_cells*);//the current cell will be set to the value from the temporary variable
void inc (char*, tmp_cells*, array*);            //the numeric value in the temporary variable will be incremented by 1
void sub (char*, tmp_cells*, array*);//subtracts the value of the _Y variable from the value in the temporary variable _X
int check_len_row (array);                               //check of maximum layer of bulky rods
bool quotes (char*, char);                               //check of the handle on the lid and the divider

int main(int argc, char* argv[]) {

    FILE* fp;
    if ((fp = fopen(argv[argc-1], "r"))==NULL) {
        fprintf(stderr,"Cannot open file.\n");
        exit(1);
    }

    char ch;                                       //variable to read a character from a file
    char* tmp = NULL;                              //variable where the string will be written
    int len = 0;                                   //length of the entered string
    char delim;                                    //the sign that will divide the cells
    array arr = {NULL, NULL, 0}; //variable with table
    command commands = {{}, 0};             //variable with commands
    char* token;                                  //auxiliary variable
    char s[2] = ";";                              //symbol that divides orders
    int idx;                                      //argument index with orders
    selection sel = {0, 0, 0, 0};  //custom cell selection variable
    selection tmp_sel = {0, 0, 0, 0};  //temporary cell selection variable
    //temporary cells
    tmp_cells tmp_cell = { {}, {false, false, false, false, false, false, false, false, false, false}};
    int remember_i = -1;  //variable for loops
    int max_cell;         //a variable that determines to which cell to write out

    //determine which character will divide the cells at the output
    if (strcmp(argv[1], "-d") == 0) {
        if (argc != 5) {
            fprintf(stderr,"Wrong number of arguments.\n");
            fclose(fp);
            exit(1);
        } else {
            delim = argv[2][0];
        }
    } else {
        if (argc != 3) {
            fprintf(stderr,"Wrong number of arguments.\n");
            fclose(fp);
            exit(1);
        } else {
            delim = ' ';
        }
    }

    tmp = tmp_ctor(tmp, &len);
    ctor_row(&arr);
    ctor_cells(&arr);

    //read the table from the file and enter it into the variable
    do {
        ch = (char) getc (fp);
        if (ch != EOF && ch != '\n') {
            tmp = tmp_realloc(tmp, &len);
            tmp[len-1] = ch;
        }

        if (ch == '\n' || ch == EOF) {
            tmp = tmp_realloc(tmp, &len);
            tmp[len - 1] = '\0';
            if (strcmp (argv[1], "-d") == 0) {
                fdelim_and_row(tmp, argv[2]);
            } else {
                fdelim_zero(tmp, delim);
            }
            arr.count++;
            realloc_row(&arr, arr.count);
            realloc_count_cells(&arr);
            row_transfer(&arr, tmp);
            tmp_dtor(tmp, &len);
            tmp = tmp_ctor(tmp, &len);
        }
    } while (ch != EOF);
    free(tmp);
    fclose(fp);
    table_alignment(&arr);

    //determine where the orders will be
    if (strcmp (argv[1], "-d") == 0) {
        idx = 3;
    }
    else {
        idx = 1;
    }
    token = strtok(argv[idx], s);

    //we split orders and put them into an array
    while (token != NULL) {
        if (strlen(token) > 1000) {
            fprintf(stderr, "Too long command.\n");
            free_arr(&arr);
            free(token);
            exit(1);
        }
        strcpy(commands.command[commands.count], token);
        commands.count++;
        token = strtok(NULL, s);
    }
    free(token);
    if (commands.count > 1000) {
        fprintf(stderr, "Too many commands");
        free_arr(&arr);
    }

    //we follow the orders entered by the user
    for (int i = 0; i < commands.count; i++) {
        if (commands.command[i][0] == '[' && commands.command[i][1] != 'm' && commands.command[i][1] != 'f'
            && commands.command[i][1] != 's' && commands.command[i][2] != ']') {
            change_selection(commands.command[i], &sel, &arr, &tmp_cell);
        }
        if (commands.command[i][0] == '[' && commands.command[i][1] == 'm' && commands.command[i][2] == 'a') {
            sel = max(sel, arr);
        }
        if (commands.command[i][0] == '[' && commands.command[i][1] == 'm' && commands.command[i][2] == 'i') {
            sel = min(sel, arr);
        }
        if (commands.command[i][0] == '[' && commands.command[i][1] == 'f') {
            sel = find(sel, arr, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'i' && commands.command[i][1] == 'r') {
            irow(&arr, sel);
        }
        if (commands.command[i][0] == 'a' && commands.command[i][1] == 'r') {
            arow(&arr, sel);
        }
        if (commands.command[i][0] == 'd' && commands.command[i][1] == 'r') {
            drow(&arr, sel);
        }
        if (commands.command[i][0] == 'i' && commands.command[i][1] == 'c') {
            icol(&arr, sel);
        }
        if (commands.command[i][0] == 'a' && commands.command[i][1] == 'c') {
            acol(&arr, sel);
        }
        if (commands.command[i][0] == 'd' && commands.command[i][1] == 'c') {
            dcol(&arr, sel);
        }
        if (commands.command[i][0] == 's' && commands.command[i][1] == 'e') {
            set(&arr, sel, commands.command[i]);
        }
        if (commands.command[i][0] == 'c' && commands.command[i][1] == 'l') {
            clear(&arr, sel);
        }
        if (commands.command[i][0] == 's' && commands.command[i][1] == 'w') {
            swap(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 's' && commands.command[i][1] == 'u' && commands.command[i][2] == 'm') {
            sum(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'a' && commands.command[i][1] == 'v') {
            avg(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'c' && commands.command[i][1] == 'o') {
            count(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'l' && commands.command[i][1] == 'e') {
            flen(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'd' && commands.command[i][1] == 'e' && commands.command[i][2] == 'f') {
            fdef(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == 'u' && commands.command[i][1] == 's' && commands.command[i][2] == 'e') {
            fuse(&arr, sel, commands.command[i], &tmp_cell);
        }
        if (commands.command[i][0] == '[' && commands.command[i][1] == '_' && commands.command[i][2] == ']') {
            sel = tmp_sel;
        }
        if (commands.command[i][0] == '[' && commands.command[i][1] == 's' && commands.command[i][2] == 'e') {
            tmp_sel = sel;
        }
        if (commands.command[i][0] == 'i' && commands.command[i][1] == 'n' && commands.command[i][2] == 'c') {
            inc(commands.command[i], &tmp_cell, &arr);
        }
        if (commands.command[i][0] == 'g' && commands.command[i][1] == 'o' && commands.command[i][5] == '+') {
            int value = commands.command[i][6] - '0';
            i += (value - 1);
        }
        if (commands.command[i][0] == 'g' && commands.command[i][1] == 'o' && commands.command[i][5] == '-') {
            int value = commands.command[i][6] - '0';
            if (i != remember_i) {
                remember_i = i;
                i -= (value + 1);
            }
        }
        if (commands.command[i][0] == 'i' && commands.command[i][1] == 's' && commands.command[i][2] == 'z') {
            int x = commands.command[i][8] - '0';
            int n = commands.command[i][11] - '0';
            if (commands.command[i][10] == '-' && tmp_cell.cells[x] != NULL) {
                if (tmp_cell.cells[x][0] == '0') {
                    i += (n - 1);
                }
            }
            if (commands.command[i][10] == '-' && tmp_cell.cells[x] != NULL) {
                if (tmp_cell.cells[x][0] == '0') {
                    remember_i = i;
                    i -= (n + 1);
                }
            }
        }
        if (commands.command[i][0] == 's' && commands.command[i][1] == 'u' && commands.command[i][2] == 'b') {
            sub(commands.command[i], &tmp_cell, &arr);
        }
    }
    max_cell = check_len_row(arr);

    printf("row_min: %d\n", sel.row_min);
    printf("row_max: %d\n", sel.row_max);
    printf("col_min: %d\n", sel.col_min);
    printf("col_max: %d\n", sel.col_max);

    for (int i = 0; i < arr.count; i ++){
        for (int j = 0; j < max_cell; j++) {
            if (quotes(arr.rows[i][j], delim)) {
                if (j != max_cell - 1)
                    printf("\"%s\"%c", arr.rows[i][j], delim);
                else
                    printf("\"%s\"\n", arr.rows[i][j]);
            } else {
                if (j != max_cell - 1)
                    printf("%s%c", arr.rows[i][j], delim);
                else
                    printf("%s\n", arr.rows[i][j]);
            }
        }
    }

    //write the table to a file
    if ((fp = fopen(argv[argc-1], "w"))==NULL) {
        fprintf(stderr,"Cannot open file.\n");
        free_arr(&arr);
        free_tmp_cell(&tmp_cell);
        exit(1);
    }

    for (int i = 0; i < arr.count; i ++){
        for (int j = 0; j < max_cell; j++) {
            if (quotes(arr.rows[i][j], delim)) {
                if (j != max_cell - 1)
                    fprintf(fp, "\"%s\"%c", arr.rows[i][j], delim);
                else
                    fprintf(fp, "\"%s\"\n", arr.rows[i][j]);
            } else {
                if (j != max_cell - 1)
                    fprintf(fp, "%s%c", arr.rows[i][j], delim);
                else
                    fprintf(fp, "%s\n", arr.rows[i][j]);
            }
        }
    }

    fclose(fp);

    //freeing memory
    free_arr(&arr);
    free_tmp_cell(&tmp_cell);
    return 0;
}

//temporary variable memory allocation
char* tmp_ctor (char* tmp, int* len) {
    *len = 0;
    char* tmp1 = malloc (1);
    tmp = tmp1;
    if (tmp == NULL) {
        fprintf(stderr, "malloc err");
        free(tmp);
        exit (1);
    }

    return tmp;
}

//changing allocated memory to a temporary variable
char* tmp_realloc (char* tmp, int* len) {
    (*len)++;
    char* tmp1 = realloc(tmp, *len+1);
    tmp = tmp1;
    if (tmp == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }

    return tmp;
}

//freeing memory of a temporary variable
void tmp_dtor (char* tmp, int* len) {
    memset(tmp, 0, *len);
    free (tmp);
    *len = 0;
}

//all delimeters are changed to \n
void fdelim (char* row, char delim) {
    int i = 0;
    bool write = true;

    while (row[i] != '\0') {
        if (row[i] == '"' && (row[i+1] == delim || row[i+1] == '\0')) {
            write = true;
        }

        if (row[i] == '"' && i == 0) {
            write = false;
        }

        if (row[i] == '"' && i != 0)
            if (row[i-1] == '\n')
                write = false;

        if (row[i] == delim && write == true) {
            row[i] = '\n';
        }

        i++;
    }
}

//remove quotation mark
void delet_quotes (char* row, int i) {
    for (int j = i; j < (int) strlen(row); j++)
        row[j] = row[j+1];
}

//adding a backslash
void write_quotes (char* row, int i) {
    int len = (int) strlen(row) + 1;
    row = realloc (row, sizeof(char*) * len);
    row[len - 1] = '\0';
    for (int j = len - 1; j > i; j--) {
        row[j] = row[j - 1];
    }
    row[i] = '\\';
}

//dividing a row into cells(if you entered -d)
void fdelim_and_row (char* row, char* delim) {
    int i = 0;
    bool write = true;

    while (delim[i] != '\0') {
        fdelim(row, delim[i]);
        i++;
    }
    i = 0;

    while (row[i] != '\0') {
        if (row[i] == '"' && (row[i+1] == '\n' || row[i+1] == '\0') && write == false) {
            delet_quotes(row, i);
            write = true;
        }

        if (row[i] == '"' && i == 0 && write == true) {
            delet_quotes(row, i);
            write = false;
        }

        if (row[i] == '"' && i != 0)
            if (row[i-1] == '\n') {
                delet_quotes(row, i);
                write = false;
            }


        if (row[i] == '"') {
            write_quotes(row, i);
            i += 1;
        }

        if (row[i] == '\\') {
            write_quotes(row, i);
            i += 1;
        }

        i++;
    }

}

//dividing a row into cells(if you not entered -d)
void fdelim_zero (char* row, char delim) {
    int  i = 0;
    bool write = true;
    fdelim(row, delim);

    while (row[i] != '\0') {
        if (row[i] == '"' && (row[i+1] == '\n' || row[i+1] == '\0') && write == false) {
            delet_quotes(row, i);
            write = true;
        }

        if (row[i] == '"' && i == 0 && write == true) {
            delet_quotes(row, i);
            write = false;
        }

        if (row[i] == '"' && i != 0)
            if (row[i-1] == '\n') {
                delet_quotes(row, i);
                write = false;
            }

        if (row[i] == '"') {
            write_quotes(row, i);
            i += 1;
        }

        if (row[i] == '\\') {
            write_quotes(row, i);
            i += 1;
        }

        i++;
    }
}

//memory allocation line
void ctor_row (array* arr) {
    arr->rows = malloc (sizeof(char***));
    if (arr->rows == NULL) {
        fprintf(stderr, "malloc err");
        exit (1);
    }
}

//cell memory allocation
void ctor_cell (array* arr, int row) {
    arr->rows[row-1] = malloc (sizeof(char**));
    if (arr->rows[row-1] == NULL) {
        fprintf(stderr, "malloc err");
        exit (1);
    }
}

//word memory allocation
void ctor_length (array* arr, int row, int cell, int length) {
    char* tmp = malloc (sizeof(char*) * length);
    arr->rows[row-1][cell-1] = tmp;
    if (arr->rows[row-1][cell-1] == NULL) {
        fprintf(stderr, "malloc err");
        exit (1);
    }
}

//change allocated word memory
void realloc_length (array* arr, int row, int cell, int length) {
    char* tmp = realloc (arr->rows[row-1][cell-1], sizeof(char*) * length);
    arr->rows[row-1][cell-1] = tmp;
    if (arr->rows[row-1][cell-1] == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }
    arr->rows[row - 1][cell - 1][length] = '\0';
}

//memory allocation for the number of cells in a row
void ctor_cells (array* arr) {
    arr->cells = malloc (sizeof(int**));
    if (arr->cells == NULL) {
        fprintf(stderr, "malloc err");
        exit (1);
    }
}

//changing allocated memory for the number of cells in a row
void realloc_count_cells (array* arr) {
    int** tmp = realloc (arr->cells, sizeof(int**) * (arr->count+1));
    arr->cells = tmp;
    if (arr->cells == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }
}

//changing the memory allocated to a row
void realloc_row (array* arr, int count_row) {
    char*** tmp = realloc(arr->rows, sizeof(char***) * count_row);
    arr->rows = tmp;
    if (arr->rows == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }
}

//changing allocated memory for cells
void realloc_cell (array* arr, int row, int count_cell) {
    char** tmp = realloc (arr->rows[row-1], sizeof(char**) * count_cell);
    arr->rows[row-1] = tmp;
    if (arr->rows[row-1] == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }
}

//change allocated memory for information of the number of cells
void realloc_cells (array* arr, int row, int length) {
    int *tmp = realloc (arr->cells[row-1], sizeof(int*) * length);
    arr->cells[row-1] = tmp;
    if (arr->cells[row-1] == NULL) {
        fprintf(stderr, "realloc err");
        exit (1);
    }
}

//entering a string into cells
void row_transfer (array* arr, char* tmp) {
    char* tmp_cell = NULL;
    int count_cell = 0;
    char delim = '\n';
    ctor_cell(arr, arr->count);

    tmp_cell = strtok(tmp, &delim);
    while( tmp_cell != NULL ) {
        count_cell++;
        realloc_cell(arr, arr->count, count_cell);
        ctor_length(arr, arr->count, count_cell, sizeof(tmp_cell) + 1);
        strcpy(arr->rows[arr->count-1][count_cell-1], tmp_cell);
        memset(tmp_cell, 0, strlen(tmp_cell));
        tmp_cell = strtok(NULL, &delim);
    }
    arr->cells[arr->count - 1] = malloc(sizeof(count_cell));
//    realloc_cells(arr, arr->count, sizeof(count_cell));
    *arr->cells[arr->count - 1] = count_cell;

    free (tmp_cell);
}

//freeing memory in a table
void free_arr (array* arr) {
    for (int i = arr->count - 1; i >= 0; i--) {
        for (int j = *arr->cells[i] - 1; j >= 0; j--) {
            free(arr->rows[i][j]);
        }
        free(arr->cells[i]);
        free (arr->rows[i]);
    }
    free (arr->cells);
    free(arr->rows);
}

//freeing memory in temporary locations
void free_tmp_cell (tmp_cells* tmp_cell) {
    for (int i = 0; i < 10; i++) {
        if (tmp_cell->use[i])
            free (tmp_cell->cells[i]);
    }
}

//table alignment
void table_alignment (array* arr) {
    int max = 0;
    for (int i = 0; i < arr->count; i++) {
        if (max < *arr->cells[i])
            max = *arr->cells[i];
    }

    for (int i = 0; i < arr->count; i++) {
        if (max > *arr->cells[i]) {
            arr->rows[i] = realloc(arr->rows[i], sizeof(char**) * max + 100);
            for (int j = *arr->cells[i]; j < max; j++) {
                arr->rows[i][j] = malloc (sizeof(char*));
                arr->rows[i][j][0] = '\0';
            }
            *arr->cells[i] = max;
        }
    }
}

//cell selection
void change_selection (char* command, selection* sel, array* arr, tmp_cells* tmp_cell) {
    sel->row_min = 0;
    sel->row_max = 0;
    sel->col_min = 0;
    sel->col_max = 0;
    int i = 1;
    int choose = 1;
    while (command[i] != ']') {
        if (command[i] == ',') {
            choose++;
        }
        else {
            switch (choose) {
                case 1:
                    if (command[i] == '_') {
                        sel->row_min = 1;
                        sel->row_max = arr->count;
                    }
                    else
                        sel->row_min = sel->row_min * 10 + (command[i] - '0');
                    break;
                case 2:
                    if (command[i] == '_') {
                        sel->col_min = 1;
                        sel->col_max = *arr->cells[sel->row_min - 1];
                    } else {
                        sel->col_min = sel->col_min * 10 + (command[i] - '0');
                    }
                    break;
                case 3:
                    if (command[i] == '-')
                        sel->row_max = arr->count;
                    else
                        sel->row_max = sel->row_max * 10 + (command[i] - '0');
                    break;
                case 4:
                    if (command[i] == '-')
                        sel->col_max = *arr->cells[0];
                    else
                        sel->col_max = sel->col_max * 10 + (command[i] - '0');
                    break;
                default:
                    break;
            }
        }

        i++;
    }

    if (sel->row_max == 0) {
        sel->row_max = sel->row_min;
    }

    if (sel->col_max == 0) {
        sel->col_max = sel->col_min;
    }

    if (sel->col_max > *arr->cells[0]) {
        for (int j = *arr->cells[0]; j < sel->col_max; j++) {
            selection tmp = {arr->count, arr->count, *arr->cells[0], *arr->cells[0]};
            acol(arr, tmp);
        }
    }

    if (sel->row_max > arr->count) {
        for (int j = arr->count; j < sel->row_max; j++) {
            selection tmp = {arr->count, arr->count, *arr->cells[0], *arr->cells[0]};
            arow(arr, tmp);
        }
    }

    if (sel->col_max < 0 || sel->row_min < 0 || sel->col_min < 0 || sel->row_max < 0) {
        fprintf(stderr, "Selected cell cannot be negative.\n");
        free_arr(arr);
        free_tmp_cell(tmp_cell);
        exit(1);
    }

    if (sel->col_max < sel->col_min) {
        fprintf(stderr, "Cell selection error.\n");
        free_arr(arr);
        free_tmp_cell(tmp_cell);
        exit(1);
    }

    if (sel->row_max < sel->row_min) {
        fprintf(stderr, "Cell selection error.\n");
        free_arr(arr);
        free_tmp_cell(tmp_cell);
        exit(1);
    }
}

//finds a cell with the maximum numeric value and sets the selection to it
selection max (selection sel, array arr) {
    int number = atoi(arr.rows[sel.row_min - 1][sel.col_min - 1]);
    selection remember = {0, 0, 0, 0};
    for (int i = sel.row_min - 1; i < sel.row_max; i++) {
        for (int j = sel.col_min - 1; j < sel.col_max; j++) {
            if (number <= atoi(arr.rows[i][j])) {
                number = atoi(arr.rows[i][j]);
                remember.row_min = i + 1;
                remember.row_max = i + 1;
                remember.col_min = j + 1;
                remember.col_max = j + 1;
            }
        }
    }

    return remember;
}

//finds a cell with the minimum numeric value and sets the selection to it
selection min (selection sel, array arr) {
    int number = atoi(arr.rows[sel.row_min - 1][sel.col_min - 1]);
    selection remember = {0, 0, 0, 0};
    for (int i = sel.row_min - 1; i < sel.row_max; i++) {
        for (int j = sel.col_min - 1; j < sel.col_max; j++) {
            if (number >= atoi(arr.rows[i][j])) {
                number = atoi(arr.rows[i][j]);
                remember.row_min = i + 1;
                remember.row_max = i + 1;
                remember.col_min = j + 1;
                remember.col_max = j + 1;
            }
        }
    }

    return remember;
}

//selects the first cell whose value contains the substring STR
selection find (selection sel, array arr, char* commands, tmp_cells* tmp_cell) {
    int k = 6;
    selection remember = {0, 0, 0, 0};
    for (int i = sel.row_min - 1; i < sel.row_max; i++) {
        for (int j = sel.col_min - 1; j < sel.col_max; j++) {
            for (int len = 0; len < (int) strlen(arr.rows[i][j]); len++){
                if (commands[k] == ']') {
                    remember.row_min = i + 1;
                    remember.row_max = i + 1;
                    remember.col_min = j + 1;
                    remember.col_max = j + 1;
                    return remember;
                }

                if (commands[k] != arr.rows[i][j][len]) {
                    k = 5;
                }
                k++;
            }
        }
    }

    fprintf(stderr, "Cell not found.\n");
    free_arr(&arr);
    free_tmp_cell(tmp_cell);
    exit(1);
}

//inserts one blank row above the selected cells
void irow (array* arr, selection sel) {
    arr->count++;
    realloc_row(arr, arr->count);
    realloc_count_cells(arr);
    arr->cells[arr->count - 1] = arr->cells[0];
    ctor_cell(arr, arr->count);
    realloc_cell(arr, arr->count, *arr->cells[0]);

    for (int i = 0; i < *arr->cells[0]; i++) {
        ctor_length(arr, arr->count, i + 1, 2);
    }

    for (int i = arr->count - 2; i >= (sel.row_min - 1); i--) {
        for (int j = 0; j < *arr->cells[i]; j++) {
            realloc_length(arr, i + 2, j + 1, sizeof(arr->rows[i][j]));
            strcpy(arr->rows[i + 1][j], arr->rows[i][j]);
        }
    }

    for (int i = 0; i < *arr->cells[0]; i++) {
        realloc_length(arr, sel.row_min, i + 1, 2);
        arr->rows[sel.row_min - 1][i][0] = '\0';
    }
    realloc_count_cells(arr);
    arr->cells[arr->count - 1] = malloc(sizeof(*arr->cells[0]));
    *arr->cells[arr->count - 1] = *arr->cells[0];
}

//adds one blank row below the selected cells
void arow (array* arr, selection sel) {
    arr->count++;
    realloc_row(arr, arr->count);
    realloc_count_cells(arr);
    arr->cells[arr->count - 1] = arr->cells[0];
    ctor_cell(arr, arr->count);
    realloc_cell(arr, arr->count, *arr->cells[0]);

    for (int i = 0; i < *arr->cells[0]; i++) {
        ctor_length(arr, arr->count, i + 1, 2);
    }

    for (int i = arr->count - 2; i >= sel.row_max; i--) {
        for (int j = 0; j < *arr->cells[i]; j++) {
            realloc_length(arr, i + 2, j + 1, sizeof(arr->rows[i][j]));
            strcpy(arr->rows[i + 1][j], arr->rows[i][j]);
        }
    }

    for (int i = 0; i < *arr->cells[0]; i++) {
        realloc_length(arr, sel.row_max + 1, i + 1, 2);
        arr->rows[sel.row_max][i][0] = '\0';
    }
    realloc_count_cells(arr);
    arr->cells[arr->count - 1] = malloc(sizeof(*arr->cells[0]));
    *arr->cells[arr->count - 1] = *arr->cells[0];
}

//deletes the selected rows
void drow (array* arr, selection sel) {
    int max_row = arr->count;
    for (int k = sel.row_min; k <= sel.row_max; k++) {
        for (int i = sel.row_min - 1; i < arr->count - 1; i++) {
            for (int j = 0; j < *arr->cells[i]; j++) {
                realloc_length(arr, i + 1, j + 1, sizeof(arr->rows[i + 1][j]));
                strcpy(arr->rows[i][j], arr->rows[i + 1][j]);
            }
        }
        arr->count--;
    }
    for (int i = max_row - 1; i > (arr->count - 1); i--) {
        for (int j = *arr->cells[i] - 1; j >= 0; j--) {
            free(arr->rows[i][j]);
        }
        free(arr->cells[i]);
        free (arr->rows[i]);
    }
}

//inserts one empty column to the left of the selected cells
void icol (array* arr, selection sel) {
    for (int i = 0; i < arr->count; i++) {
        *arr->cells[i]+= 1;
        realloc_cell(arr, i + 1, *arr->cells[i]);
        ctor_length(arr, i + 1, *arr->cells[i], 2);
        arr->rows[i][*arr->cells[i] - 1][0] = '\0';
    }

    for (int i = 0; i < arr->count; i++) {
        for (int j = *arr->cells[i] - 1; j >= sel.col_min; j--) {
            realloc_length(arr, i + 1, j + 1, sizeof(arr->rows[i][j - 1]));
            strcpy(arr->rows[i][j], arr->rows[i][j - 1]);
        }
    }

    for (int i = 0; i < arr->count; i++) {
        realloc_length(arr, i + 1, sel.col_min, 2);
        arr->rows[i][sel.col_min - 1][0] = '\0';
    }
}

//adds one empty column to the right of the selected cells
void acol (array* arr, selection sel) {
    for (int i = 0; i < arr->count; i++) {
        *arr->cells[i]+= 1;
        realloc_cell(arr, i + 1, *arr->cells[i]);
        ctor_length(arr, i + 1, *arr->cells[i], 2);
        arr->rows[i][*arr->cells[i] - 1][0] = '\0';
    }

    for (int i = 0; i < arr->count; i++) {
        for (int j = *arr->cells[i] - 1; j > sel.col_max; j--) {
            realloc_length(arr, i + 1, j + 1, sizeof(arr->rows[i][j - 1]));
            strcpy(arr->rows[i][j], arr->rows[i][j - 1]);
        }
    }

    for (int i = 0; i < arr->count; i++) {
        realloc_length(arr, i + 1, sel.col_max + 1, 2);
        arr->rows[i][sel.col_max][0] = '\0';
    }
}

//deletes the selected columns
void dcol (array* arr, selection sel) {
    for (int k = sel.col_min; k <= sel.col_max; k++) {
        for (int i = 0; i < arr->count; i++) {
            for (int j = sel.col_min; j < *arr->cells[i]; j++) {
                realloc_length(arr, i + 1, j, sizeof(arr->rows[i][j]));
                strcpy(arr->rows[i][j - 1], arr->rows[i][j]);
            }
            *arr->cells[i] -= 1;
            if (*arr->cells[i] != 0) {
                free(arr->rows[i][*arr->cells[i]]);
                realloc_cell(arr, i + 1, *arr->cells[i]);
            }
        }
    }
}

void set (array* arr, selection sel, char* commands) {
    char del[2] = " ";
    commands = strtok(commands, del);
    commands = strtok(NULL, del);

    if (commands[0] == '"') {
        delet_quotes(commands, 0);
        delet_quotes(commands, (int) strlen(commands) - 1);
    }

    for (int j = sel.row_min; j <= sel.row_max; j++) {
        for (int k = sel.col_min; k <= sel.col_max; k++) {
            realloc_length(arr, j, k, sizeof(commands));
            strcpy(arr->rows[j - 1][k - 1], commands);
        }
    }
}

//the contents of the selected cells will be deleted
void clear (array* arr, selection sel) {
    for (int j = sel.row_min; j <= sel.row_max; j++) {
        for (int k = sel.col_min; k <= sel.col_max; k++) {
            realloc_length(arr, j, k, 2);
            arr->rows[j - 1][k - 1][0] = '\0';
        }
    }
}

//replaces the contents of the selected cell
void swap (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    selection tmp_sel = {0, 0, 0, 0};
    char* tmp = NULL;
    char del[2] = " ";
    int len = sizeof(arr->rows[sel.row_min - 1][sel.col_min - 1]);
    commands = strtok(commands, del);
    commands = strtok(NULL, del);

    change_selection(commands, &tmp_sel, arr, tmp_cell);
    for (int j = sel.row_min; j <= sel.row_max; j++) {
        for (int k = sel.col_min; k <= sel.col_max; k++) {
            tmp = malloc(sizeof(char*) * len);
            strcpy(tmp, arr->rows[j - 1][k - 1]);

            realloc_length(arr, j, k, sizeof(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1]));
            strcpy(arr->rows[j - 1][k - 1], arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1]);

            realloc_length(arr, tmp_sel.row_min, tmp_sel.col_min, sizeof(tmp));
            strcpy(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1], tmp);
        }
    }
    free (tmp);
}

//stores the sum of the values of the selected cells
void sum (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    selection tmp_sel = {0, 0, 0, 0};
    char del[2] = " ";
    int sum = 0;
    char* cell_sum = NULL;
    commands = strtok(commands, del);
    commands = strtok(NULL, del);
    change_selection(commands, &tmp_sel, arr, tmp_cell);

    for (int i = sel.row_min; i <= sel.row_max; i++) {
        for (int j = sel.col_min; j <= sel.col_max; j++) {
            sum += atoi (arr->rows[i - 1][j - 1]);
        }
    }
    sprintf(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1], "%g", (double) sum);

    free (cell_sum);
}

//same as sum, but the arithmetic mean is stored
void avg (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    selection tmp_sel = {0, 0, 0, 0};
    char del[2] = " ";
    float sum = 0;
    char* cell_sum = NULL;
    commands = strtok(commands, del);
    commands = strtok(NULL, del);
    change_selection(commands, &tmp_sel, arr, tmp_cell);

    for (int i = sel.row_min; i <= sel.row_max; i++) {
        for (int j = sel.col_min; j <= sel.col_max; j++) {
            sum += atoi (arr->rows[i - 1][j - 1]);
        }
    }
    sum /= (sel.row_max - sel.row_min + 1) * (sel.col_max - sel.col_min + 1);

    sprintf(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1], "%g", (double) sum);
    free (cell_sum);
}

//same as sum, but the number of non-empty cells is stored
void count (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    selection tmp_sel = {0, 0, 0, 0};
    char del[2] = " ";
    int sum = 0;
    char* cell_sum = NULL;
    commands = strtok(commands, del);
    commands = strtok(NULL, del);

    change_selection(commands, &tmp_sel, arr, tmp_cell);

    for (int i = sel.row_min; i <= sel.row_max; i++) {
        for (int j = sel.col_min; j <= sel.col_max; j++) {
            if (arr->rows[i-1][j-1][0] != '\0')
                sum++;
        }
    }

    sprintf(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1], "%g", (double) sum);
    free (cell_sum);
}

//stores the string length of the currently selected cell
void flen (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    selection tmp_sel = {0, 0, 0, 0};
    char del[2] = " ";
    int len_old = 0;
    int len_new = 0;
    commands = strtok(commands, del);
    commands = strtok(NULL, del);

    change_selection(commands, &tmp_sel, arr, tmp_cell);

    len_old = strlen(arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1]);
    len_new = strlen(arr->rows[sel.row_min - 1][sel.col_min - 1]);

    realloc_length(arr, tmp_sel.row_min, tmp_sel.col_min, len_new);

    for (int i = len_old; i < len_new; i++) {
        arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1][i] = ' ';
    }
    arr->rows[tmp_sel.row_min - 1][tmp_sel.col_min - 1][len_new] = '\0';
}

//the value of the current cell will be set to a temporary variable
void fdef (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    char del[2] = " ";
    commands = strtok(commands, del);
    commands = strtok(NULL, del);
    int value = commands[1] - '0';

    if (value >= 0 && 9 >= value) {
        tmp_cell->use[value] = true;
        tmp_cell->cells[value] = malloc(sizeof(char*) * strlen(arr->rows[sel.row_min - 1][sel.col_min - 1]));
        strcpy(tmp_cell->cells[value], arr->rows[sel.row_min - 1][sel.col_min - 1]);
    } else {
        fprintf(stderr, "This temporary variable does not exist.\n");
        free_arr(arr);
        free_tmp_cell(tmp_cell);
        exit(1);
    }
}

//the current cell will be set to the value from the temporary variable
void fuse (array* arr, selection sel, char* commands, tmp_cells* tmp_cell) {
    char del[2] = " ";
    commands = strtok(commands, del);
    commands = strtok(NULL, del);
    int value = commands[1] - '0';

    if (value >= 0 && 9 >= value) {
        for (int j = sel.row_min; j <= sel.row_max; j++) {
            for (int k = sel.col_min; k <= sel.col_max; k++) {
                arr->rows[j - 1][k - 1] = realloc(arr->rows[j - 1][k - 1],
                                                  sizeof(char*) * strlen(tmp_cell->cells[value]));
                strcpy(arr->rows[j - 1][k - 1], tmp_cell->cells[value]);
            }
        }
    } else {
        fprintf(stderr, "This temporary variable does not exist.\n");
        free_arr(arr);
        free_tmp_cell(tmp_cell);
        exit(1);
    }
}

//the numeric value in the temporary variable will be incremented by 1
void inc (char* command, tmp_cells* tmp_cell, array* arr) {
    char* cell_value = NULL;
    int number = command[5] - '0';
    int value = atoi (tmp_cell->cells[number]);
    int len = 0;
    int coefficient;
    value++;

    if (number >= 0 && number <= 9) {
        cell_value = malloc(sizeof(char*) * len);
        if (value >= 0) {
            coefficient = 1;
        } else {
            coefficient = -1;
        }
        while (value != 0) {
            int piece = value % 10;
            cell_value = tmp_realloc(cell_value, &len);
            cell_value[len - 1] = (piece * coefficient) + '0';
            value /= 10;
        }
        if (coefficient == -1) {
            cell_value[len] = '-';
            cell_value = tmp_realloc(cell_value, &len);
        }
        cell_value[len] = '\0';

        for (int i = 0; i < (len / 2); i++) {
            char number = cell_value[i];
            cell_value[i] = cell_value[len - 1 - i];
            cell_value[len - 1 - i] = number;
        }

        tmp_cell->cells[number] = realloc(tmp_cell->cells[number], sizeof(char*) * strlen(cell_value));
        strcpy(tmp_cell->cells[number], cell_value);
    } else {
        fprintf(stderr, "This temporary variable does not exist.\n");
        free_tmp_cell(tmp_cell);
        free_arr(arr);
        free (cell_value);
        exit(1);
    }
    free (cell_value);
}

//subtracts the value of the _Y variable from the value in the temporary variable _X
void sub (char* command, tmp_cells* tmp_cell, array* arr) {
    int number1 = command[5] - '0';
    int value1 = atoi (tmp_cell->cells[number1]);
    int number2 = command[8] - '0';
    int value2 = atoi (tmp_cell->cells[number2]);
    char* cell_value = NULL;
    int len = 0;
    int coefficient;
    value1 -= value2;

    if (number1 >= 0 && number1 <= 9 && number2 >= 0 && number2 <= 9) {
        cell_value = malloc(sizeof(char*) * len);
        if (value1 >= 0) {
            coefficient = 1;
        } else {
            coefficient = -1;
        }
        while (value1 != 0) {
            int piece = value1 % 10;
            cell_value = tmp_realloc(cell_value, &len);
            cell_value[len - 1] = (piece * coefficient) + '0';
            value1 /= 10;
        }
        if (coefficient == -1) {
            cell_value[len] = '-';
            cell_value = tmp_realloc(cell_value, &len);
        }
        cell_value[len] = '\0';

        for (int i = 0; i < (len / 2); i++) {
            char number = cell_value[i];
            cell_value[i] = cell_value[len - 1 - i];
            cell_value[len - 1 - i] = number;
        }

        tmp_cell->cells[number1] = realloc(tmp_cell->cells[number1], sizeof(char*) * strlen(cell_value));
        strcpy(tmp_cell->cells[number1], cell_value);
    } else {
        fprintf(stderr, "This temporary variable does not exist.\n");
        free_tmp_cell(tmp_cell);
        free_arr(arr);
        free (cell_value);
        exit(1);
    }
    free (cell_value);
}

//check of maximum layer of bulky rods
int check_len_row (array arr) {
    int len = 0;
    for (int i = 0; i < arr.count; i++) {
        int tmp_len = 0;
        for (int j = 0; j < *arr.cells[i]; j++) {
            if (arr.rows[i][j][0] != '\0') {
                tmp_len = j + 1;
            }
        }
        if (tmp_len > len) {
            len = tmp_len;
        }
    }

    return len;
}

//check of the handle on the lid and the divider
bool quotes (char* cell, char delim) {
    int i = 0;
    while (cell[i] != '\0') {
        if (cell[i] == delim)
            return true;
        if (cell[i] == '"')
            return true;
        i++;
    }
    return false;
}
