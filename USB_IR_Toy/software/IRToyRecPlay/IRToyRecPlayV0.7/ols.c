#include "ols.h"



void create_ols( char *param_fname)
{
  int i,flag;
  unsigned long absolute=0;
  unsigned long io=0;
  unsigned long size=0;
  unsigned long dtw=0;
  unsigned long decimalno=0;
  int fcounter;
  char inkey;
 // char *param_delay=NULL;

  char fnameseq[255],fnameseq_ols[255];
  FILE *fp, *fp_ols=NULL;
  int res;
  uint16_t ols_buffer[1];
  char s[4];
      //check filename if exist
        printf(" Entering OLS conversion Mode \n");
        fcounter=0;
        inkey=0;
        while (1) {
            sprintf(fnameseq,"%s_%03d.bin",param_fname,fcounter);
            fp=fopen(fnameseq,"rb");
            if (fp==NULL) {
                if (fcounter > 0)
                    printf(" No more file(s). \n\n");
                else
                    printf(" OLS File does not exits. \n");
               break;
            }

            sprintf(fnameseq_ols,"%s_%03d.ols",param_fname,fcounter);
            fp_ols=fopen(fnameseq_ols,"w");
            if (fp_ols==NULL) {
                   printf(" Error: Cannot create OLS file: %s. \n",fnameseq_ols);
               break;
            }

            printf("\n Creating OLS file: %s ",fnameseq_ols);
            size=0;
            absolute=0;
            char temp[4];
            flag=0;
            while(!feof(fp)) {
                  if ((res=fread(&ols_buffer,sizeof(uint16_t),sizeof(ols_buffer),fp)) > 0) {
                       for(i=0;i<res;i++) {
                          sprintf(temp,"%04X",(uint16_t) ols_buffer[i]);
                          sprintf(s,"%c%c%c%c",temp[2],temp[3],temp[0],temp[1]);
                          decimalno = strtoul(s, 0, 16);
                     //     printf("i= %i, temp= %s  s= %s  Decimalno= %lu size= %lu absoulte= %lu res=%i\n ",i,temp,s,decimalno,size,absolute,res);
                          if(decimalno < 65535) {
                             size++;
                             absolute=absolute+decimalno;
                          }
                       } //for

                   } //if


            }  //while
            //printf("size = %lu,  Absolute = %lu\n",size,absolute+200);
            fprintf(fp_ols,";Size: %lu\n",size);
            fprintf(fp_ols,";Rate: 213333\n");
            fprintf(fp_ols,";Channels: 32\n");
            fprintf(fp_ols,";EnabledChannels: 255\n");
            fprintf(fp_ols,";TriggerPosition: 0\n");
            fprintf(fp_ols,";Compressed: true\n");
            fprintf(fp_ols,";AbsoluteLength: %lu\n",absolute+200);
            fprintf(fp_ols,";CursorEnabled: false\n");
            fprintf(fp_ols,";Cursor0: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor1: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor2: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor3: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor4: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor5: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor6: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor7: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor8: -9223372036854775808\n");
            fprintf(fp_ols,";Cursor9: -9223372036854775808\n");
            fseek(fp, 0L, SEEK_SET);
            io=0;
            dtw=0;
             while(!feof(fp)) {
                if ((res=fread(&ols_buffer,sizeof(uint16_t),sizeof(ols_buffer),fp)) > 0) {
                   char temp[4];
                   for(i=0;i<res;i++) {

                       sprintf(temp,"%04X",(uint16_t) ols_buffer[i]);
                       // reverse
                       sprintf(s,"%c%c%c%c",temp[2],temp[3],temp[0],temp[1]);
                     //  printf("temp2 = %s, s2 = %s\n",temp,s);
                       decimalno = strtoul(s, 0, 16);
                       if (io == 0) {
                            if (decimalno < 65535) {
                                dtw = dtw + decimalno;
                                fprintf(fp_ols,"00000000@%lu\n",dtw);
                                io = 1;
                            }
                        }
                        else {
                            if (decimalno < 65535) {
                                dtw = dtw+decimalno;
                                fprintf(fp_ols,"00000001@%lu\n",dtw);
                                io = 0;
                            }
                        }
                   }
                }

               }

           printf(" .. Done.\n");
           fclose(fp);
           fclose(fp_ols);
           fcounter++;
        }
}