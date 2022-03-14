#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

unsigned long long SEED = 312891;
unsigned long long RANV;
int RANI = 0;

double Ranq1();
void normal(double sig, double *n1, double *n2);
double CHK(double L1, double L2);
double table[8] = {0.65, 0.55, 0.45, 0.35, 0.25, 0.15, 0.05, 0};

int main() {
    // for declaration
    int i, j, m;                  //for counting
    int n, rc;              // n is column  = 7376 and rc is row = 3688
    int dv1, dv2, dc1, dc2; // dv: column have #1 and dc: row have #1
    
    int **L1 = NULL;                 // check node connect 6 variable nodes
    int L1lenrow = rc/2;        // rc = 3688/2 = 1844
    int L1lencolumn = dc1;     // dc = 12
    int **L2 = NULL;           // check node connect 6 variable nodes
    int L2lenrow = rc/2;        // rc = 3688/2
    int L2lencolumn = dc2;     // dc = 6

    int **M1 = NULL;
    int M1lenrow = n/2;       // n = 7376/2
    int M1lencolumn = dv1;   // dv1 = 3
    int **M2 = NULL;
    int M2lenrow = n/2;       // n = 7376/2
    int M2lencolumn = dv2;   // dv2 = 6

    int *codarray;          //codeword, Dynamic memory allocation, length = codarraylen
    int codarraylen = n;
    int *codearray;         //0->1; 1->-1,  Dynamic memory allocation, length = codearraylen
    int codearraylen = n;
    double *outp;           // codeword + noise, Dynamic memory allocation, length = outparray
    int outparray = n;
    int *output;            // result of interative algorithm decoding, Dynamic memory allocation, length = outputarray
    int outputarray = n;
    double *Lj;             // LLR
    int Ljlen = n;
    
    
    double **qij1 = NULL;    // from down to top
    int qij1row = dv1;        // qijrow = dv1 = 3
    int qij1column = n/2;      // qijcolumn = n = 7376/2
    double **qij2 = NULL;    // from down to top
    int qij2row = dv2;        // qijrow = dv2 = 6
    int qij2column = n/2;      // qijcolumn = n = 7376/2

    double **uij1 = NULL;    // from top to down
    int uij1row = rc/2;        // uijrow = rc = 3688/2
    int uij1column = dc1;     // uijcloumn = dc = 12
    double **uij2 = NULL;    // from top to down
    int uij2row = rc/2;        // uijrow = rc = 3688
    int uij2column = dc2;     // uijcloumn = dc = 6
    
    double tempqij1[11];         // bottom - up
    double tempqij2[5]; 
    double tempuij;
    
    double temp1uij1[3];         // top - down
    double temp1uij2[6];

    int *comput;            // bottom - up
    int computlen = n;      // computlen = 7376
    int *comput1;           // top - down
    int comput1len = rc;    // comput1len = 3688
    
    int *comput2;
    int comput2len = rc;    // comput2len = 3688
    
    double *qj;
    int qjlen = n;
    int *checkbit;
    int checkbitlen = rc;
    int krc;
    int **G;
    int Glenrow = krc;
    int Glencolumn = n;
    int *u;
    int ulen = krc;

    //int step;               // test 6 EbN0(dB) result
    int s = 0;              // compute numbers of syndrome is same as zero
    int s_2 = 0;            // compute level = 2

    int error_block = 0;    // receive 100 error block
    int num = 0;            // do compute transmit how many block
    int I_eq=0;             // equivalent iterations; initialize = 0 
    int I_max=100;           // now, test max iteration = 20
    int I_eq_1_2 = 0;           // compute level1 no level2
    int sgn;                    // judge positive or negative

    int level = 2;          // uep ldpc separate into two part
    int k;                  // set k = level l
    k = level;
    printf("k = %d; \n", k);

    int error1;
    int error2;

    int totalerror1=0;
    int totalerror2=0;

    int restart = 0;
    double sigma;
    double ebn0;
    double ebn0s;
    double bers1=0;
    double bers2=0;
    double berscompare;

    double x, y;
    int valL;
    int valL2;
    double app;
    double app1;
    int stp;

    int temp;
    // declaration end
    ebn0s= 1.8;

    // open file
    FILE *fpr;
    fpr=fopen("C1parchematrix.txt","r");
    fscanf(fpr,"%d",&n);
    fscanf(fpr,"%d",&rc);
    printf("column = %d\n", n);
    printf("row = %d\n", rc);
    fscanf(fpr,"%d",&dc1);
    fscanf(fpr,"%d",&dc2);
    fscanf(fpr,"%d",&dv1);
    fscanf(fpr,"%d",&dv2);
    printf("dc1 = %d\n", dc1); 
    printf("dc2 = %d\n", dc2);
    printf("dv1 = %d\n", dv1);
    printf("dv2 = %d\n", dv2);
    L1lenrow = rc/2;    
    L1lencolumn = dc1;
    L2lenrow = rc/2;
    L2lencolumn = dc2;
    M1lenrow = n/2; 
    M1lencolumn = dv1;
    M2lenrow = n/2; 
    M2lencolumn = dv2;

    M1 = (int **)malloc(M1lenrow * sizeof(int *));
    for (i = 0; i < M1lenrow; i++) M1[i] = (int *)malloc(M1lencolumn * sizeof(int));
    M2 = (int **)malloc(M1lenrow * sizeof(int *));
    for (i = 0; i < M2lenrow; i++) M2[i] = (int *)malloc(M2lencolumn * sizeof(int));
    L1 = (int **)malloc(L1lenrow * sizeof(int *));
    for (i = 0; i < L1lenrow; i++) L1[i] = (int *)malloc(L1lencolumn * sizeof(int));
    L2 = (int **)malloc(L2lenrow * sizeof(int *));
    for (i = 0; i < L2lenrow; i++) L2[i] = (int *)malloc(L2lencolumn * sizeof(int));

    for (j = 0; j < M1lenrow; j++) {
        for (i = 0; i < M1lencolumn; i++) {
            fscanf(fpr,"%d",&M1[j][i]);
        }
    }
    for (j = 0; j < M2lenrow; j++) {
        for (i = 0; i < M2lencolumn; i++) {
            fscanf(fpr,"%d",&M2[j][i]);
        }
    }
    //int temp;
    for (j = 0; j < M1lenrow; j++) {
        for (i = 0; i < M1lencolumn; i++) {
            for (m = i; m < M1lencolumn; m++) {
                if (M1[j][m] < M1[j][i]) {
                    temp = M1[j][m];
                    M1[j][m] = M1[j][i];
                    M1[j][i] = temp;
                }
            }
        }
    }
    for (j = 0; j < M2lenrow; j++) {
        for (i = 0; i < M2lencolumn; i++) {
            for (m = i; m < M2lencolumn; m++) {
                if (M2[j][m] < M2[j][i]) {
                    temp = M2[j][m];
                    M2[j][m] = M2[j][i];
                    M2[j][i] = temp;
                }
            }
        }
    }
    for (i = 0; i < L1lenrow; i++) {
        for (j = 0; j < L1lencolumn; j++) {
            fscanf(fpr,"%d",&L1[i][j]);
        }
    }
    for (i = 0; i < L2lenrow; i++) {
        for (j = 0; j < L2lencolumn; j++) {
            fscanf(fpr,"%d",&L2[i][j]);
        }
    }
    for (i = 0; i < L1lenrow; i++) {
        for (j = 0; j < L1lencolumn; j++) {
            for (m = j; m < L1lencolumn; m++) {
                if (L1[i][m] < L1[i][j]) {
                    temp = L1[i][m];
                    L1[i][m] =L1[i][j];
                    L1[i][j] = temp;
                }
            }
        }
    }
    for (i = 0; i < L2lenrow; i++) {
        for (j = 0; j < L2lencolumn; j++) {
            for (m = j; m < L2lencolumn; m++) {
                if (L2[i][m] < L2[i][j]) {
                    temp = L2[i][m];
                    L2[i][m] = L2[i][j];
                    L2[i][j] = temp;
                }
            }
        }
    }
    fclose(fpr);
    
    // close file
    
    // open file
    FILE *fpr1;
    fpr1=fopen("generator1.txt","r");
    n = 7376;
    krc = 3688;
    printf("column = %d\n", n);
    printf("row = %d\n", krc);
    Glenrow = krc;
    Glencolumn = n;
    G = (int **)malloc(Glenrow * sizeof(int *));
    for (i = 0; i < Glenrow; i++) G[i] = (int *)malloc(Glencolumn * sizeof(int));
    for (i = 0; i < Glenrow; i++) {
        for (j = 0; j < Glencolumn; j++) fscanf(fpr1,"%d",&G[i][j]);
    }
    fclose(fpr1);
    // close file
    // open declare
    codarraylen = n;
    codarray = (int *)malloc(codarraylen * sizeof(int));
    if( codarray == NULL ) {
        // 無法取得記憶體空間
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return 1;
    }
    codearraylen = n;
    codearray = (int *)malloc( codearraylen * sizeof(int) );
    if( codearray == NULL ) {
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return 1;
    }
    outparray = n;
    outp = (double *)malloc( outparray * sizeof(double) );
    if (outp == NULL) {
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return 1;
    }
    outputarray = n;
    output = (int *)malloc( outputarray * sizeof(int) );
    if (output == NULL) {
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return 1;
    }
    Ljlen = n;
    Lj = (double *)malloc(Ljlen * sizeof(double));
    if (Lj == NULL) {
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return 1;
    }
    qij1row = dv1;        // qijrow = dv = 3
    qij1column = n/2;      // qijcolumn = n = 816
    qij1 = (double **)malloc(qij1row * sizeof(double *));
    for (i = 0; i < qij1row; i++) qij1[i] = (double *)malloc(qij1column * sizeof(double));
    qij2row = dv2;        // qijrow = dv = 3
    qij2column = n/2;      // qijcolumn = n = 816
    qij2 = (double **)malloc(qij2row * sizeof(double *));
    for (i = 0; i < qij2row; i++) qij2[i] = (double *)malloc(qij2column * sizeof(double));
    
    uij1row = rc/2;        // uijrow = rc = 408
    uij1column = dc1;     // uijcolumn = dc = 6 
    uij1 = (double **)malloc(uij1row * sizeof(double *));
    for (i = 0; i < uij1row; i++) uij1[i] = (double *)malloc(uij1column * sizeof(double));
    uij2row = rc/2;        // uijrow = rc = 408
    uij2column = dc2;     // uijcolumn = dc = 6 
    uij2 = (double **)malloc(uij2row * sizeof(double *));
    for (i = 0; i < uij2row; i++) uij2[i] = (double *)malloc(uij2column * sizeof(double));

    computlen = n;
    comput = (int *)malloc(computlen * sizeof(int));
    comput1len = rc;
    comput1 = (int *)malloc(comput1len * sizeof(int));
    comput2len = rc;
    comput2 = (int *)malloc(comput2len * sizeof(int));

    qjlen = n;
    qj = (double *)malloc(qjlen * sizeof(double));
    checkbitlen = rc;
    checkbit = (int *)malloc(checkbitlen * sizeof(int));

    ulen = krc;
    u = (int *)malloc(ulen * sizeof(int));

    // for CODE part
    while (error_block < 100) {        
        k = 2;   // need to initialize for computing new information block
        I_eq = 0;
        k=level;
        
        printf("error_block = %d\n", error_block);
        // initial
        for (i = 0; i < codarraylen; i++) {
            codarray[i] = 0;
        }
        // produce random transfer information data
        
        if (num == 0) {
            u[0] = 1;
            u[1] = 0;
            u[2] = 0;
            u[3] = 0;
            u[4] = 0;
            u[5] = 0;
            for (i = 0; i < krc - 6; i++) u[i + 6] = (u[i + 1] + u[i]) % 2;
        } else {
            u[0] = (u[krc-5] + u[krc-6]) % 2;
            u[1] = (u[krc-4] + u[krc-5]) % 2;
            u[2] = (u[krc-3] + u[krc-4]) % 2;
            u[3] = (u[krc-2] + u[krc-3]) % 2;
            u[4] = (u[krc-1] + u[krc-2]) % 2;
            u[5] = (u[krc-0] + u[krc-1]) % 2;
            for (i = 0; i < krc - 6; i++) u[i + 6] = (u[i + 1] + u[i]) % 2;
        }
        num++;  // compute transmit how many times of data
        // compute x = u*G
        for (i = 0; i < ulen; i++) {
            if (u[i] == 1) {
                for (j = 0; j < n; j++) codarray[j] = (codarray[j] + G[i][j]) % 2;
            }
        }
        //input to AWGN channel normalized to +-1
        for (i = 0; i < codearraylen; i++) {
            if (codarray[i] == 0) codearray[i] = 1;
            else codearray[i] = -1;
        }
        ebn0 = ebn0s;
        sigma = sqrt(1.0 / (pow(10, ebn0/10)));
        // add noise to code
        for(i = 0; i < rc; i++) {
            normal(sigma, &x, &y);
            outp[2 * i] = codearray[2 * i] + x;
            outp[2 * i + 1] = codearray[2 * i + 1] + y;
        }
        //printf("yes\n");
        // the log likelihood ratio (LLR)
        ebn0 = pow(10, ebn0/10);
        for(i = 0; i < Ljlen; i++) Lj[i] =4 * 0.5 * ebn0 * outp[i];     //  0.5 * 1.2544 = Es/N0
        
        // the interative decoding algotrithm
        for (j = 0; j < qij1column; j++) {                               // initialization
            for (i = 0; i < qij1row; i++) qij1[i][j] = Lj[j]; 
        }
        for (j = 0; j < qij2column; j++) {                               // initialization
            for (i = 0; i < qij2row; i++) qij2[i][j] = Lj[3688 + j]; 
        }
        for (i = 0; i < uij1row; i++) {
            for (j = 0; j < uij1column; j++) uij1[i][j] = 0;
        }
        for (i = 0; i < uij2row; i++) {
            for (j = 0; j < uij2column; j++) uij2[i][j] = 0;
        }
        // in k=level, still compute all algorithm
        
        while (I_eq < I_max && k == level) {
            k = level;
            s = 0;
            s_2 = 0;
            for (i = 0; i < 11; i++) tempqij1[i] = 0.0;
            for (i = 0; i < 5; i++) tempqij2[i] = 0.0;
            for (i = 0; i < computlen; i++) comput[i] = 0;
            // bottom - up
            for (i = 0; i < rc; i++) {
                if (i < (rc/2)) {
                    for (j = 0; j < 12; j++) {
                        for (m = 0; m < 11; m++) {
                            if (m < j) {
                                valL = L1[i][m]-1;
                                if (valL < (n/2)) tempqij1[m] = qij1[comput[valL]][valL];
                                else tempqij1[m] = qij2[comput[valL]][valL - 3688];
                            } else if (m >= j) {
                                valL = L1[i][m+1]-1;
                                if (valL < (n/2)) tempqij1[m] = qij1[comput[valL]][valL];
                                else tempqij1[m] = qij2[comput[valL]][valL - 3688];
                            }
                        }
                        tempuij = tempqij1[0];
                        for(m = 1; m < 11; m++) tempuij = CHK(tempuij, tempqij1[m]);
                        uij1[i][j] = tempuij;
                    }
                } else {
                    for (j = 0; j < 6; j++) {
                        for (m = 0; m < 5; m++) {
                            if (m < j) {
                                valL = L2[i-1844][m]-1;
                                if (valL < (n/2)) tempqij2[m] = qij1[comput[valL]][valL];
                                else tempqij2[m] = qij2[comput[valL]][valL-3688];
                            } else if (m >= j) {
                                valL = L2[i-1844][m+1]-1;
                                if (valL < (n/2)) tempqij2[m] = qij1[comput[valL]][valL];
                                else tempqij2[m] = qij2[comput[valL]][valL-3688];
                            }
                        }
                        tempuij = tempqij2[0];
                        for(m = 1; m < 5; m++) tempuij = CHK(tempuij, tempqij2[m]);
                        uij2[i-1844][j] = tempuij;
                    }
                }
                if (i < (rc/2)) {
                    for (m = 0; m < 12; m++) comput[L1[i][m] - 1] += 1;
                } else {
                    for (m = 0; m < 6; m++) comput[L2[i-1844][m] - 1] += 1;
                }
            }
            // top-down
            for(i = 0; i < 3; i++) temp1uij1[i] = 0.0;
            for(i = 0; i < 6; i++) temp1uij2[i] = 0.0;
            for (i = 0; i < comput1len; i++) comput1[i] = 0;
            for (j = 0; j < n; j++) {
                if (j < (n/2)) {
                    for (i = 0; i < 3; i++) {
                        for (m = 0; m < 2; m++) {
                            if (m < i) { 
                                valL = M1[j][m] - 1;
                                if (valL < (rc/2)) temp1uij1[m] = uij1[valL][comput1[valL]];
                                else  temp1uij1[m] = uij2[valL-1844][comput1[valL]];
                            } else if (m >= i) {
                                valL = M1[j][m + 1] - 1;
                                if (valL < (rc/2)) temp1uij1[m] = uij1[valL][comput1[valL]];
                                else  temp1uij1[m] = uij2[valL-1844][comput1[valL]];
                            }
                        }
                        temp1uij1[2] = Lj[j];
                        qij1[i][j] = temp1uij1[0] + temp1uij1[1] + temp1uij1[2];
                    }
                } else {
                    for (i = 0; i < 6; i++) {
                        for (m = 0; m < 5; m++) {
                            if (m < i) { 
                                valL = M2[j-3688][m] - 1;
                                if (valL < (rc/2)) temp1uij2[m] = uij1[valL][comput1[valL]]; 
                                else temp1uij2[m] = uij2[valL-1844][comput1[valL]]; 
                            } else if (m >= i) {
                                valL = M2[j-3688][m + 1] - 1;
                                if (valL < (rc/2)) temp1uij2[m] = uij1[valL][comput1[valL]]; 
                                else temp1uij2[m] = uij2[valL-1844][comput1[valL]];
                            }
                        }
                        temp1uij2[5] = Lj[j];
                        qij2[i][j-3688] = temp1uij2[0] + temp1uij2[1] + temp1uij2[2] +temp1uij2[3]+temp1uij2[4]+temp1uij2[5];
                    }
                }
                if (j < n/2) {
                    for (m = 0; m < 3; m++) comput1[M1[j][m] - 1] += 1;
                } else {
                    for (m =0; m < 6; m++) comput1[M2[j-3688][m] - 1] += 1;
                }
            }
            //
            // decision
            for (i = 0; i < comput2len; i++) comput2[i] = 0;
            for (j = 0; j < n; j++) {
                qj[j] = Lj[j];
                if (j < (n/2)) {
                    for (i = 0; i < 3; i++) {
                        valL = M1[j][i] - 1;
                        if (valL < (rc/2)) qj[j] += uij1[valL][comput2[valL]];
                        else qj[j] += uij2[valL-1844][comput2[valL]];
                    }
                    if (qj[j] >= 0) output[j] = 0;
                    else if (qj[j] < 0) output[j] = 1;
                    for (i = 0; i < 3; i++) comput2[M1[j][i] - 1] += 1;
                } else {
                    for (i = 0; i < 6; i++) {
                        valL = M2[j-3688][i] - 1;
                        if (valL < (rc/2)) qj[j] += uij1[valL][comput2[valL]];
                        else qj[j] += uij2[valL-1844][comput2[valL]];
                    }
                    if (qj[j] >= 0) output[j] = 0;
                    else if (qj[j] < 0) output[j] = 1;
                    for (i = 0; i < 6; i++) comput2[M2[j-3688][i] - 1] += 1;
                }
            }
            // compute the partial syndrome s(k) 
            // to check Hx=0     
            for (i = 0; i < rc; i++) {
                checkbit[i] = 0;
                if (i < (rc/2)) {
                    for (j = 0; j < 12; j++) checkbit[i] += output[L1[i][j] - 1];
                    checkbit[i] = checkbit[i] % 2;
                } else {
                    for (j = 0; j < 6; j++) checkbit[i] += output[L2[i-1844][j] - 1];
                    checkbit[i] = checkbit[i] % 2;
                }
            }
            for (i= rc/2; i < rc; i++) 
                if (checkbit[i] == 0) {
                    s_2++;
                    s++;        // wait for correct
                }    
            for (i = 0; i < rc/2; i++) if (checkbit[i] == 0 ) s++;
            I_eq++;
            printf("I_eq = %d\n", I_eq);
            if (s_2 == rc/2) k--;
            //printf("k = %d\n",k);
        }
        error2 = 0;
        for (i = n/2; i < n; i++) {
            if((output[i] != codarray[i])) error2++;            
        }
        printf("k = %d\n",k);
        while (I_eq < I_max && k == level - 1 && s != rc/2) {
            s = 0;
            for (I_eq_1_2 = 0; I_eq_1_2 < 3; I_eq_1_2++) {
                //s = 0;
                for (i = 0; i < 11; i++) tempqij1[i] = 0.0;
                for (i = 0; i < 5; i++) tempqij2[i] = 0.0;
                for (i = 0; i < computlen; i++) comput[i] = 0;
                // bottom - up
                for (i = 0; i < rc; i++) {
                    if (i < (rc/2)) {
                        for (j = 0; j < 12; j++) {
                            for (m = 0; m < 11; m++) {
                                if (m < j) {
                                    valL = L1[i][m]-1;
                                    if (valL < (n/2)) tempqij1[m] = qij1[comput[valL]][valL];
                                    else tempqij1[m] = qij2[comput[valL]][valL - 3688];
                                } else if (m >= j) {
                                    valL = L1[i][m+1]-1;
                                    if (valL < (n/2)) tempqij1[m] = qij1[comput[valL]][valL];
                                    else tempqij1[m] = qij2[comput[valL]][valL - 3688];
                                }
                            }
                            tempuij = tempqij1[0];
                            if (j <6){
                                for (m = 1; m < 5; m++) tempuij = CHK(tempuij, tempqij1[m]);
                                for (m = 5; m < 11; m++) {
                                    if (tempqij1[m] > 0) sgn = 1;
                                    else if (tempqij1[m] == 0) sgn = 0;
                                    else sgn = -1;
                                    //if (i == 0) printf("sgn=%d ", sgn);
                                    tempuij = sgn * tempuij;
                                }
                            } else {
                                for (m = 1; m < 6; m++) tempuij = CHK(tempuij, tempqij1[m]);
                                for (m = 6; m < 11; m++) {
                                    if (tempqij1[m] > 0) sgn = 1;
                                    else if (tempqij1[m] == 0) sgn = 0;
                                    else sgn = -1;
                                    tempuij = sgn * tempuij;
                                }
                            }
                            //for(m = 1; m < 11; m++) tempuij = CHK(tempuij, tempqij1[m]);
                            uij1[i][j] = tempuij;
                        }
                    } /*else {
                        for (j = 0; j < 6; j++) {
                            for (m = 0; m < 5; m++) {
                                if (m < j) {
                                    valL = L2[i-1844][m]-1;
                                    if (valL < (n/2)) tempqij2[m] = qij1[comput[valL]][valL];
                                    else tempqij2[m] = qij2[comput[valL]][valL-3688];
                                } else if (m >= j) {
                                    valL = L2[i-1844][m+1]-1;
                                    if (valL < (n/2)) tempqij2[m] = qij1[comput[valL]][valL];
                                    else tempqij2[m] = qij2[comput[valL]][valL-3688];
                                }
                            }
                            tempuij = tempqij2[0];
                            for(m = 1; m < 5; m++) {
                                tempuij = CHK(tempuij, tempqij2[m]);
                                if (tempqij1[m] > 0) sgn = 1;
                                else if (tempqij1[m] == 0) sgn = 0;
                                else sgn = -1;
                            }
                            uij2[i-1844][j] = tempuij;
                        }
                        
                    }*/
                    if (i < (rc/2)) {
                        for (m = 0; m < 12; m++) comput[L1[i][m] - 1] += 1;
                    } else {
                        for (m = 0; m < 6; m++) comput[L2[i-1844][m] - 1] += 1;
                    }
                }
                // top-down
                for(i = 0; i < 3; i++) temp1uij1[i] = 0.0;
                for(i = 0; i < 6; i++) temp1uij2[i] = 0.0;
                for (i = 0; i < comput1len; i++) comput1[i] = 0;
                for (j = 0; j < n; j++) {
                    if (j < (n/2)) {
                        for (i = 0; i < 3; i++) {
                            for (m = 0; m < 2; m++) {
                                if (m < i) { 
                                    valL = M1[j][m] - 1;
                                    if (valL < (rc/2)) temp1uij1[m] = uij1[valL][comput1[valL]];
                                    else  temp1uij1[m] = uij2[valL-1844][comput1[valL]];
                                } else if (m >= i) {
                                    valL = M1[j][m + 1] - 1;
                                    if (valL < (rc/2)) temp1uij1[m] = uij1[valL][comput1[valL]];
                                    else  temp1uij1[m] = uij2[valL-1844][comput1[valL]];
                                }
                            }
                            temp1uij1[2] = Lj[j];
                            qij1[i][j] = temp1uij1[0] + temp1uij1[1] + temp1uij1[2];
                        }
                    } /*else {
                        for (i = 0; i < 6; i++) {
                            for (m = 0; m < 5; m++) {
                                if (m < i) { 
                                    valL = M2[j-3688][m] - 1;
                                    if (valL < (rc/2)) temp1uij2[m] = uij1[valL][comput1[valL]]; 
                                    else temp1uij2[m] = uij2[valL-1844][comput1[valL]]; 
                                } else if (m >= i) {
                                    valL = M2[j-3688][m + 1] - 1;
                                    if (valL < (rc/2)) temp1uij2[m] = uij1[valL][comput1[valL]]; 
                                    else temp1uij2[m] = uij2[valL-1844][comput1[valL]];
                                }
                            }
                            temp1uij2[5] = Lj[j];
                            qij2[i][j-3688] = temp1uij2[0] + temp1uij2[1] + temp1uij2[2] +temp1uij2[3]+temp1uij2[4]+temp1uij2[5];
                        }
                    }*/
                    if (j < n/2) for (m = 0; m < 3; m++) comput1[M1[j][m] - 1] += 1;
                    else for (m =0; m < 6; m++) comput1[M2[j-3688][m] - 1] += 1;
                }
            }    //
            // decision
            
            for (i = 0; i < comput2len; i++) comput2[i] = 0;
            for (j = 0; j < n; j++) {
                qj[j] = Lj[j];
                if (j < (n/2)) {
                    for (i = 0; i < 3; i++) {
                        valL = M1[j][i] - 1;
                        if (valL < (rc/2)) qj[j] += uij1[valL][comput2[valL]];
                        else qj[j] += uij2[valL-1844][comput2[valL]];
                    }
                    if (qj[j] >= 0) output[j] = 0;
                    else if (qj[j] < 0) output[j] = 1;
                    for (i = 0; i < 3; i++) comput2[M1[j][i] - 1] += 1;
                } else {
                    /*for (i = 0; i < 6; i++) {
                        valL = M2[j-3688][i] - 1;
                        if (valL < (rc/2)) qj[j] += uij1[valL][comput2[valL]];
                        else qj[j] += uij2[valL-1844][comput2[valL]];
                    }
                    if (qj[j] >= 0) output[j] = 0;
                    else if (qj[j] < 0) output[j] = 1;*/
                    for (i = 0; i < 6; i++) comput2[M2[j-3688][i] - 1] += 1;
                }
            }

            // compute the partial syndrome s(k) 
            // to check Hx=0     
            for (i = 0; i < rc; i++) {
                checkbit[i] = 0;
                if (i < (rc/2)) {
                    for (j = 0; j < 12; j++) checkbit[i] += output[L1[i][j] - 1];
                    checkbit[i] = checkbit[i] % 2;
                } else {
                    for (j = 0; j < 6; j++) checkbit[i] += output[L2[i-1844][j] - 1];
                    checkbit[i] = checkbit[i] % 2;
                }
            }
                /*for (i= rc/2; i < rc; i++) 
                    if (checkbit[i] == 0) {
                        s_2++;
                        s++;
                    }  */  
            for (i = 0; i < rc/2; i++) if (checkbit[i] == 0 ) s++;
            
            I_eq++;
            printf("I_eq = %d\n", I_eq);
            //if (s_2 == rc/2) k--;
            //printf("k = %d\n",k);
        }
        error1 = 0;
        // compute error block have number of error bits
        for (i = 0; i < n/2 ; i++) {
            if (output[i] != codarray[i]) error1++;
        }
        if (error1 != 0) error_block++;
        totalerror1+=error1;
        totalerror2+=error2;
        printf("num = %d ",num);
        printf("totalerror1 = %d ", totalerror1);
        printf("totalerror2 = %d ", totalerror2);
    }
    double ber1;
    double ber2;
    ber1 = (double)totalerror1 / (num * n / 2);
    ber2 = (double)totalerror2 / (num * n / 2);
    printf("totalerror1 = %d\n", totalerror1);
    printf("totalerror2 = %d\n", totalerror2);
    printf("BER1 = %g\n", ber1);
    printf("BER2 = %g\n", ber2);


    // code ended

    FILE *outfp2; 
    outfp2 = fopen("c2alg_1_8_level1_imax=100.txt","w");
         fprintf(outfp2,"%g ",ebn0s);
         fprintf(outfp2,"%g ",ber1);
         fprintf(outfp2,"%g ",ber2);
         fprintf(outfp2,"\n");
    fclose(outfp2);
    // free dynametic
    free(codarray);
    free(codearray);
    free(outp);
    free(output);
    for (i = 0; i < L1lenrow; i++) free(L1[i]);
    free(L1);
    for (i = 0; i < M1lenrow; i++) free(M1[i]);
    free(M1);
    for (i = 0; i < L2lenrow; i++) free(L2[i]);
    free(L2);
    for (i = 0; i < M2lenrow; i++) free(M2[i]);
    free(M2);

    for (i = 0; i < qij1row; i++) free(qij1[i]);
    free(qij1);
    for (i = 0; i < uij1row; i++) free(uij1[i]);
    free(uij1);
    for (i = 0; i < qij2row; i++) free(qij2[i]);
    free(qij2);
    for (i = 0; i < uij2row; i++) free(uij2[i]);
    free(uij2);
    free(Lj);
    free(comput);
    free(comput1);
    free(comput2);
    for (i = 0; i < krc; i++) free(G[i]);
    free(G);
    free(u);
    return 0;
}
void normal(double sig, double *n1, double *n2)
{   
    double x1,x2;
    double s;
    //printf("sigma = %g\n", sig);
    do{
        x1 = Ranq1();
        x2 = Ranq1();
        x1 = 2 * x1 - 1;
        x2 = 2 * x2 - 1;
        s = x1 * x1 + x2 * x2;
    } while (s >= 1.0);
    *n1 = sig * x1 * sqrt((-2.0 * log(s))/ s);
    *n2 = sig * x2 * sqrt((-2.0 * log(s))/ s);
    
}

double Ranq1() {
    if ( RANI == 0 ){
        RANV = SEED ^ 4101842887655102017LL;
        RANV ^= RANV >> 21;
        RANV ^= RANV << 35;
        RANV ^= RANV >> 4;
        RANV = RANV * 2685821657736338717LL;
        RANI++;
    }
    RANV ^= RANV >> 21;
    RANV ^= RANV << 35;
    RANV ^= RANV >> 4;

    return RANV * 2685821657736338717LL * 5.42101086242752217E-20;
}

double CHK(double L1, double L2) 
{   
    double sgn1, sgn2, min;

    if(L1>0) sgn1 = 1;
    else if(L1 == 0) sgn1 = 0;
    else sgn1 = -1;
    if(L2>0) sgn2 = 1;
    else if(L2 == 0) sgn2 = 0;
    else sgn2 = -1;
    if(fabs(L1) >= fabs(L2)) min = fabs(L2);
    else min = fabs(L1);
    double temp1,temp2;
    double ope1,ope2;
    temp1 = L1+L2;
    temp2 = L1-L2;
    ope1 = fabs(temp1);
    ope2 = fabs(temp2);
    double lnup, lndown;
    if (ope1 >= 0 && ope1 < 0.196) lnup = table[0];
    else if (ope1 >= 0.196 && ope1 < 0.433) lnup = table[1];
    else if (ope1 >= 0.433 && ope1 < 0.71) lnup = table[2];
    else if (ope1 >= 0.71 && ope1 < 1.05) lnup = table[3];
    else if (ope1 >= 1.05 && ope1 < 1.508) lnup = table[4];
    else if (ope1 >= 1.508 && ope1 < 2.252) lnup = table[5];
    else if (ope1 >= 2.252 && ope1 < 4.5) lnup = table[6];
    else if (ope1 >= 4.5) lnup = table[7];

    if (ope2 >= 0 && ope2 < 0.196) lndown = table[0];
    else if (ope2 >= 0.196 && ope2 < 0.433) lndown = table[1];
    else if (ope2 >= 0.433 && ope2 < 0.71) lndown = table[2];
    else if (ope2 >= 0.71 && ope2 < 1.05) lndown = table[3];
    else if (ope2 >= 1.05 && ope2 < 1.508) lndown = table[4];
    else if (ope2 >= 1.508 && ope2 < 2.252) lndown = table[5];
    else if (ope2 >= 2.252 && ope2 < 4.5) lndown = table[6];
    else if (ope2 >= 4.5) lndown = table[7];

    return sgn1 * sgn2 * min +lnup-lndown /*answer*/;
}