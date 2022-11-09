#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "data.h"


/* Constants */
#define WINDOW_WIDTH 28
#define MAXCHAR 4096
/* --------- */


int Is_File_Exists(char * f_name)
{
	FILE * file;
	
	if ((file = fopen(f_name, "r")))
	{
		fclose(file);
		return 1;
	}

	return 0;
}

void Get_CSV_Data_Image(char *file_name, struct DataSet * data_set)
{
	printf("\n-------- Getting Data... --------\n\n");

    FILE *fp;
    char row[MAXCHAR];
    char *token;

    fp = fopen(file_name,"r");

    int r = 0;
    while (feof(fp) != true && r <= data_set->length)
    {
        char label[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        char * fgets_return = fgets(row, MAXCHAR, fp);
        if (fgets_return[0] == 0)
            printf("BUG\n");

        token = strtok(row, ",");

        int i = 0;
        int img_pixel_index = 0;

        while(token != NULL)
        {
            if (r >= 1 && i == 1)
            {
                label[0] = token[0];
                data_set->data_set[r-1].label[0] = token[0];
            }

            if (r >= 1 && i >= 2)
            {
                if (((float)strtol(token, (char **)NULL, 10) / 255) >= 0.25 && file_name[0] != 'X')
                {
                    data_set->data_set[r-1].input[img_pixel_index] = 1;
                }
				else if (file_name[0] == 'X')
				{
					data_set->data_set[r-1].input[img_pixel_index] = token[0]-'0';
				}
				printf("INPUT : %d\n", token[0]-'0');
                img_pixel_index++;
            }
            token = strtok(NULL, ",");
            i++;
        }
        if (r >= 1 && label[0]-'0' >= 0)
		{
            data_set->data_set[r-1].expected_output[label[0]-'0'] = 1;
			printf("EXPECTED OUTPUT : %d\n", label[0]-'0');
		}
        r++;	
    }

	printf("\n---------------------------------\n\n");
}
