/*------------------------------------------------------------------------*/

#include "rdjpeg.h"
#include "cgabor.h"

/*------------------------------------------------------------------------*/

float cgabor(CIMAGE cim, float sigma, float theta, float lambda)
{
    int i,j,k,l,w,nx,ny;
    float **im,**hr,**hi,*cr,*ci,*cn,cs;
    double sr,si,eg;
    /*--------------------------------------------------------------------*/
    w = (int) ceil(3.0*sigma);
    nx = cim.nx;
    ny = cim.ny;
    if ((nx < 2*w) || (ny < 2*w)) return(0.0);
    im = (float **) malloc(nx*sizeof(float *));
    hr = (float **) malloc(nx*sizeof(float *));
    hi = (float **) malloc(nx*sizeof(float *));
    im[0] = (float *) malloc(nx*ny*sizeof(float));
    hr[0] = (float *) malloc(nx*ny*sizeof(float));
    hi[0] = (float *) malloc(nx*ny*sizeof(float));
    for (i = 1; i < nx; i++)  {
        im[i] = im[i-1]+ny;
        hr[i] = hr[i-1]+ny;
        hi[i] = hi[i-1]+ny;
    }
    for (i = 0; i < nx; i++)  {
        for (j = 0; j < ny; j++) {
            im[i][j] = (0.30*cim.r[i][j]+0.51*cim.g[i][j]+0.19*cim.b[i][j])/255.0;
        }
    }
    eg = 0.0;
    /*--------------------------------------------------------------------*/
    cr = ((float *) malloc((2*w+1)*sizeof(float)))+w;
    ci = ((float *) malloc((2*w+1)*sizeof(float)))+w;
    cn = ((float *) malloc((2*w+1)*sizeof(float)))+w;
    for (cs = 0.0, k = -w; k <= w; k++) {
        cs += (cn[k] = exp(-0.5*(k/sigma)*(k/sigma))/(sqrt(2*M_PI)*sigma));
    }
    for (k = -w; k <= w; k++) cn[k] /= cs;
    /*--------------------------------------------------------------------*/
    for (k = -w; k <= w; k++) {
        cr[k] = cn[k]*cos(2*M_PI*k*cos(theta)/lambda);
        ci[k] = cn[k]*sin(2*M_PI*k*cos(theta)/lambda);
    }
    for (i = 0; i < w; i++)  {
        for (cs = 0.0, k = -i; k <= w; k++) cs += cn[k];
        for (j = 0; j < ny; j++) {
            sr = si = 0.0;
            for (k = -i; k <= w; k++) {
                sr += cr[k]*im[i+k][j];
                si += ci[k]*im[i+k][j];
            }
            hr[i][j] = sr/cs;
            hi[i][j] = si/cs;
        }
    }
    for (i = w; i < nx-w; i++)  {
        for (j = 0; j < ny; j++) {
            sr = si = 0.0;
            for (k = -w; k <= w; k++) {
                sr += cr[k]*im[i+k][j];
                si += ci[k]*im[i+k][j];
            }
            hr[i][j] = sr;
            hi[i][j] = si;
        }
    }
    for (i = nx-w; i < nx; i++)  {
        for (cs = 0.0, k = -w; k <= (nx-1-i); k++) cs += cn[k];
        for (j = 0; j < ny; j++) {
            sr = si = 0.0;
            for (k = -w; k <= (nx-1-i); k++) {
                sr += cr[k]*im[i+k][j];
                si += ci[k]*im[i+k][j];
            }
            hr[i][j] = sr/cs;
            hi[i][j] = si/cs;
        }
    }
    /*--------------------------------------------------------------------*/
    for (l = -w; l <= w; l++) {
        cr[l] = cn[l]*cos(2*M_PI*l*sin(theta)/lambda);
        ci[l] = cn[l]*sin(2*M_PI*l*sin(theta)/lambda);
    }
    for (j = 0; j < w; j++)  {
        for (cs = 0.0, l = -j; l <= w; l++) cs += cn[l];
        for (i = 0; i < nx; i++) {
            sr = si = 0.0;
            for (l = -j; l <= w; l++) {
                sr += cr[l]*hr[i][j+l]-ci[l]*hi[i][j+l];
                si += ci[l]*hr[i][j+l]+cr[l]*hi[i][j+l];
            }
            eg += (sr*sr+si*si)/(cs*cs);
        }
    }
    for (j = w; j < ny-w; j++)  {
        for (i = 0; i < nx; i++) {
            sr = si = 0.0;
            for (l = -w; l <= w; l++) {
                sr += cr[l]*hr[i][j+l]-ci[l]*hi[i][j+l];
                si += ci[l]*hr[i][j+l]+cr[l]*hi[i][j+l];
            }
            eg += sr*sr+si*si;
        }
    }
    for (j = ny-w; j < ny; j++)  {
        for (cs = 0.0, l = -w; l <= (ny-1-j); l++) cs += cn[l];
        for (i = 0; i < nx; i++) {
            sr = si = 0.0;
            for (l = -w; l <= (ny-1-j); l++) {
                sr += cr[l]*hr[i][j+l]-ci[l]*hi[i][j+l];
                si += ci[l]*hr[i][j+l]+cr[l]*hi[i][j+l];
            }
            eg += (sr*sr+si*si)/(cs*cs);
        }
    }
    /*--------------------------------------------------------------------*/
    free(im[0]);
    free(hr[0]);
    free(hi[0]);
    free(im);
    free(hr);
    free(hi);
    free(cr-w);
    free(ci-w);
    free(cn-w);
    return((float) sqrt(eg/(nx*ny)));
}

float *egabor(CIMAGE cim, int ndir, int nsca, float sigma0, float lambda0,
float scale)
{
    int i,j;
    float *eg,sigma,lambda,theta;
    eg = (float *) malloc(ndir*nsca*sizeof(float));
    for (i = 0; i < ndir; i++) {
        theta = i*M_PI/ndir;
        sigma = sigma0;
        lambda = lambda0;
        for (j = 0; j < nsca; j++) {
            eg[i*nsca+j] = cgabor(cim,sigma,theta,lambda);
            printf("%10.6f",eg[i*nsca+j]);
            fflush(stdout);
            sigma *= scale;
            lambda *= scale;
        }
        printf("\n");
    }
    return(eg);
}

void plotgabor(float *eg, int ndir, int nsca, char *name)
{
    int i,j;
    unsigned char c;
    float egmax = 0.0;
    FILE *fp;
    for (i = 0; i < ndir; i++) for (j = 0; j < nsca; j++)
        if (egmax < eg[i*nsca+j]) egmax = eg[i*nsca+j];
    if ((fp = fopen(name,"wb")) == NULL) exit(1);
    fprintf(fp,"P5\n%d %d\n255\n",32*nsca,32*ndir);
    for (i = 0; i < 32*ndir; i++) for (j = 0; j < 32*nsca; j++) {
        c = (unsigned char) 255.0*eg[(i/32)*nsca+(j/32)]/egmax;
        fputc(c,fp);
    }
    fclose(fp);
}

/*------------------------------------------------------------------------*/
