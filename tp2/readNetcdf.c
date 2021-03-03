#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <omp.h>

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}

/* nombre del archivo a leer */
#define FILE_NAME "OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc"

#define OUT_FILE "file.nc"

/* Lectura de una matriz de 21696 x 21696 */
#define NX 21696
#define NY 21696
#define filter_width 3
short filter[3][3] = {{ -1, -1, -1,},
                      { -1,  9, -1,},
                      { -1, -1, -1}};
int half_fw = (int) filter_width/2;

int main(int argc, char *argv[] )
{
    if(argc != 2) exit(-4);

    int ncid, varid;
    int ncid2, varid2;
    short *data_in = calloc (1,sizeof(short)*NX*NY);
    short *data_out = calloc (1,sizeof(short)*NX*NY);
    int retval;
    int retval2;

    if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
        ERR(retval);
    /* Obtenemos el varID de la variable CMI. */
    if ((retval = nc_inq_varid(ncid, "CMI", &varid)))
        ERR(retval);
    if ((retval2 = nc_open(OUT_FILE, NC_WRITE, &ncid2)))
        ERR(retval2);
    /* Obtenemos el varID de la variable CMI. */
    if ((retval2 = nc_inq_varid(ncid2, "CMI", &varid2)))
        ERR(retval2);
    /* Leemos la matriz. */
    if ((retval = nc_get_var_short(ncid, varid, data_in)))
        ERR(retval);

    /* aplico el filtro */
    int thread_count = atoi( argv[1] );
    printf("Se hace con %d hilos.\n",thread_count);
    omp_set_num_threads(thread_count);
    int i,j;
    short sum;
    #pragma omp parallel for collapse(2) private(i,j,sum)
    for(i = 0; i < NX; i++){
      for(j = 0; j < NY; j++){
        if(data_in[i*NX+j]==-1){
          data_out[i*NX+j]=-1;
        }else{
          sum =0;
          for(int k= -half_fw; k<= half_fw ;k++){
            for(int l= -half_fw ; l<= half_fw ;l++){
              int r = i+k;
              int c = j+l;
              r= (r<0) ? 0: r;
              c= (c<0) ? 0: c;
              r = (r>= NY) ? NY-1 :r;
              c= (c>=NX) ? NX-1 :c;
              sum += data_in[r*NX+c]*filter[k+half_fw][l+half_fw];
            }
          }
          data_out[i*NX+j]=sum;
        }
      }
    }

    /*Para guardar la imagen en el mismo formato utilizo */
    if ((retval2 = nc_put_var(ncid2, varid2, data_out)))
      ERR(retval2);

    /* Se cierra el archivo y liberan los recursos*/
    if ((retval = nc_close(ncid)))
        ERR(retval);
        free(data_in);
        free(data_out);
    return 0;
}
