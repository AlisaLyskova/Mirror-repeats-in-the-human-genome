#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *fseq, *fout;
    char *seq, c;
    long Len, i, j, k, l;
    long LMax = 300000000, DistMax = 20, Lrep = 10, Lrep_k = 1, repeat = 0, DistDens[DistMax];
    char chr[] = "chrX";
    if ((fout = fopen("chrX_MRpur.out","w")) == NULL) { printf(" no out file! "); goto quit; }
    if ((fseq = fopen("chrX.fasta","r")) == NULL) { fprintf(fout," no input file! "); goto quit; }
    if ((seq  = calloc(LMax, sizeof(char))) == NULL) { fprintf(fout," No memory! "); goto quit; }

/* Reading input sequence in FASTA format */
/*    First line */
        j = 0;
        for (;;)
        {
            c = getc(fseq);
            if (j == 0 && c != '>') { fprintf(fout," wrong file format! "); goto quit; }
            if (c == '\n') break;
            j ++;
        }
/*     The rest of file */
        Len = 0;
        for (;;)
        {
            c = getc(fseq);
            if (c == EOF) break;                               /* End of file reached */
            if (c == '\n') continue;                           /* end of line ignored */
            if (c == 'Y' || c == 'R' || c == 'S' || c == 'W' || c == 'M') c = 'N';
            if (c == 'A' || c == 'T' || c == 'G' || c == 'C' || c == 'N')
            {
                seq[Len] = c; Len++;
            }
            else { fprintf(fout," wrong letter: %c %10li", c, Len); goto quit; }
        }

/* 'i' is the left end of the second repeat*/
    for (i = Lrep; i != Len - Lrep; i++)
    {
        if (seq[i] != 'N' && seq[i] != 'C' && seq[i] != 'T')
        {
            /* 'j' is the right end of the first repeat*/
            for (j = i-1; j != i-DistMax-1; j--)
            {
                if (seq[i] == seq[j])
                {
                    /* exit the cycle after finding a repeat */
                    if (repeat != 0) goto next;

                    for (k = 1;; k++)
                    {
                        if ((seq[j-k] != seq[i+k]) || (seq[j-k] == 'N') || (seq[j-k] == 'C') || (seq[j-k] == 'T'))
                        {
                            goto next;
                        }
                        if (seq[j-k] == seq[i+k])
                        {
                            /* calculate the length of the assumed repeat */
                            Lrep_k ++;
                        }
                    }
                    next: ;
                    if (Lrep_k >= Lrep)
                    {
                        printf("%s %d %d %d %d ", chr, j-Lrep_k+1, i, Lrep_k, i-j-1);
                        fprintf(fout, "%s %d %d %d %d ", chr, j-Lrep_k+1, i, Lrep_k, i-j-1);
                        repeat = Lrep_k;

                        /* print the first shoulder*/
                        for (l = j - Lrep_k + 1; l != j+1; l++)
                        {
                            printf("%c", seq[l]);
                            fprintf(fout, "%c", seq[l]);
                        }
                        printf(" ");
                        fprintf(fout, " ");

                        /* print the second shoulder*/
                        for (l = i; l != i + Lrep_k; l++)
                        {
                            printf("%c", seq[l]);
                            fprintf(fout, "%c", seq[l]);
                        }
                        printf("\n");
                        fprintf(fout, "\n");
                    }
                    Lrep_k = 1;
                }
            }
        }
        /* skip the values of 'i' from the previous repeat */
        i += repeat;
        repeat = 0;
    }
quit:
    return 0;
}
