/*
PPM P3 editor
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct RGB {
    int r, g, b;
} RGB;

typedef struct IMAGEM {
    int b, h, maxcor;
    struct RGB **RGB;
} IMG;

IMG *ler_arq(const char *nome_arq) {
    int b, h, maxcor, i, j;
    IMG *imagem = NULL;
    char formato[4], linha[256];
    FILE *arq = fopen(nome_arq, "r");
    if(arq == NULL)
        return NULL;
    fgets(formato, 4, arq);
    if(strcmp(formato, "P3\n") != 0) {
    fprintf(stdout, "Arquino nao eh P3\n");
    exit(1);
    }
    int c = getc(arq);
    while (c == '#') {
    while (getc(arq) != '\n') ;
         c = getc(arq);
    }
    ungetc(c, arq);
    fscanf(arq, "%d %d", &b, &h);
    fscanf(arq, "%d", &maxcor);;
    imagem = (IMG*) malloc(sizeof(IMG));
    imagem->b = b;
    imagem->h = h;
    imagem->maxcor = maxcor;
    imagem->RGB = (RGB**) malloc(h*sizeof(RGB*));
    for(i=0; i<h; i++) {
        imagem->RGB[i] = (RGB*) malloc (b*sizeof(RGB));
    }
    for(i=0; i<h; i++) {
        for(j=0; j<b; j++) {
            fscanf(arq, "%d", &imagem->RGB[i][j].r);
            fscanf(arq, "%d", &imagem->RGB[i][j].g);
            fscanf(arq, "%d", &imagem->RGB[i][j].b);
        }
    }
    fclose(arq);
    return imagem;
}

void ver_cores(IMG *imagem) {
    for(int i=0; i < imagem->h; i++) {
        for(int j=0; j<imagem->b; j++) {
            if(imagem->RGB[i][j].r > imagem->maxcor )
                imagem->RGB[i][j].r = imagem->maxcor;

            if(imagem->RGB[i][j].r < 0)
                imagem->RGB[i][j].r = 0;

            if(imagem->RGB[i][j].g > imagem->maxcor )
                imagem->RGB[i][j].g = imagem->maxcor;

            if(imagem->RGB[i][j].g < 0)
                imagem->RGB[i][j].g = 0;

            if(imagem->RGB[i][j].b > imagem->maxcor )
                imagem->RGB[i][j].b = imagem->maxcor;

            if(imagem->RGB[i][j].b < 0)
                imagem->RGB[i][j].b = 0;

        }
    }
}

void filtro_PB(IMG *imagem, int h1, int b1, int h2, int b2) {
    for(int i=h1; i<h2; i++){
        for(int j=b1; j<b2; j++){
            imagem->RGB[i][j].r = (((imagem->RGB[i][j].r)+(imagem->RGB[i][j].g)+(imagem->RGB[i][j].b))/3);
            imagem->RGB[i][j].g = imagem->RGB[i][j].r;
            imagem->RGB[i][j].b = imagem->RGB[i][j].r;
        }
    }
    ver_cores(imagem);
}

void filtro_sepia(IMG *imagem, int h1, int b1, int h2, int b2) {
    for(int i=h1; i<h2; i++){
        for(int j=b1; j<b2; j++){
            imagem->RGB[i][j].r = ((imagem->RGB[i][j].r*0.393) + (imagem->RGB[i][j].g*0.769)	+ (imagem->RGB[i][j].b*0.189));
		    imagem->RGB[i][j].g = ((imagem->RGB[i][j].r*0.349) + (imagem->RGB[i][j].g*0.686)	+ (imagem->RGB[i][j].b*0.168));
		    imagem->RGB[i][j].b = ((imagem->RGB[i][j].r*0.272) + (imagem->RGB[i][j].g*0.534)	+ (imagem->RGB[i][j].b*0.131));
        }
    }
    ver_cores(imagem);
}

void filtro_brilho(IMG *imagem, int h1, int b1, int h2, int b2) {
    int brilho;
    printf("Digite o valor do brilho:\n");
    scanf("%d", &brilho);
    for(int i=h1; i<h2; i++){
        for(int j=b1; j<b2; j++){
            imagem->RGB[i][j].r += brilho;
		    imagem->RGB[i][j].g += brilho;
		    imagem->RGB[i][j].b += brilho;
        }
    }
    ver_cores(imagem);
}

void filtro_fliph(IMG *imagem, int h1, int b1, int h2, int b2) {
    int r, g, b;
    for(int i=h1; i<h2; i++){
        for(int j=b1; j<b2/2; j++){
            r = imagem->RGB[i][j].r;
            g = imagem->RGB[i][j].g;
            b = imagem->RGB[i][j].b;

            imagem->RGB[i][j].r = imagem->RGB[i][b2-j-1].r;
            imagem->RGB[i][j].g = imagem->RGB[i][b2-j-1].g;
            imagem->RGB[i][j].b = imagem->RGB[i][b2-j-1].b;

            imagem->RGB[i][b2-j-1].r = r;
            imagem->RGB[i][b2-j-1].g = g;
            imagem->RGB[i][b2-j-1].b = b;
        }
    }
}

void filtro_flipv(IMG *imagem, int h1, int b1, int h2, int b2) {
    int r, g, b;
    for(int i=h1; i<h2/2; i++){
        for(int j=b1; j<b2; j++){
            r = imagem->RGB[i][j].r;
            g = imagem->RGB[i][j].g;
            b = imagem->RGB[i][j].b;

            imagem->RGB[i][j].r = imagem->RGB[h2-i-1][j].r;
            imagem->RGB[i][j].g = imagem->RGB[h2-i-1][j].g;
            imagem->RGB[i][j].b = imagem->RGB[h2-i-1][j].b;

            imagem->RGB[h2-i-1][j].r = r;
            imagem->RGB[h2-i-1][j].g = g;
            imagem->RGB[h2-i-1][j].b = b;
        }
    }
}

void filtro_listras(IMG *imagem, int h1, int b1, int h2, int b2) {
    int l, lr, lg, lb;
    printf("Digite o valor RGB das listras:\n");
    scanf("%d %d %d", &lr, &lg, &lb);
    printf("Deseja listras horizontais a cada quantos pixels;\n");
    scanf("%d", &l);
    for(int i=h1; i<h2; i+=l){
        for(int j=b1; j<b2; j++){
            imagem->RGB[i][j].r = lr;
		    imagem->RGB[i][j].g = lg;
		    imagem->RGB[i][j].b = lb;
        }
    }
}

void salvar_arquivo_ppm_p3(const char *nome_arquivo, IMG *imagem) {
    FILE *arq = fopen(nome_arquivo, "w");
    int i, j;
    fprintf(arq, "P3\n%d %d\n%d\n", imagem->b, imagem->h, imagem->maxcor);
    for (i = 0; i < imagem->h; i++) {
        for (j = 0; j < imagem->b; j++) {
            fprintf(arq, "%d ", imagem->RGB[i][j].r);
            fprintf(arq, "%d ", imagem->RGB[i][j].g);
            fprintf(arq, "%d\n", imagem->RGB[i][j].b);
        }
    }
    fclose(arq);
}

int main() {
    IMG* img;
    int b1,  h1,  b2,  h2, op;
    char sn, nome_img[42], nome_img_novo[42];
    printf("Entre com o nome do arquivo:\n");
    scanf("%s", &nome_img);
    img = ler_arq(nome_img);
    do{
        printf("Deseja selecionar uma area para aplicar o filtro? (s/n)\n");
        scanf(" %c", &sn);
        if (sn == 's') {
            do{
                printf("\nEntre com h1 e b1: ");
                scanf("%d %d", &h1, &b1);
            } while (h1<0 || b1<0 || img->h < h1 || img->b < b1);
            do{
                printf("\nEntre com h2 e b2: ");
                scanf("%d %d", &h2, &b2);
            } while (h2<h1 || b2<b1 || h2 > img->h || b2 > img->b);
        } else {
            printf("1");
            h1=0;
            b1=0;
            h2=img->h;
            b2=img->b;
            printf("2");
        }
    } while (sn != 's' && sn != 'n');
    do{
        system("clear||cls");
        printf("\n|--------Menu de Filtros--------|\n");
        printf("|      1) Preto e branco.\n");
        printf("|      2) Ajuste de brilho.\n");
        printf("|      3) Filtro sepia.\n");
        printf("|      4) Flip horizontal.\n");
        printf("|      5) Flip vertical.\n");
        printf("|      6) Listras coloridas.\n");
        printf("|      0) Sair do programa.\n");
        printf("|---------------------------------|\n");
        scanf("%i", &op);
        switch (op) {
        case 1:
            filtro_PB(img, h1, b1, h2, b2);
            break;

        case 2:
            filtro_brilho(img, h1, b1, h2, b2);
            break;

        case 3:
            filtro_sepia(img, h1, b1, h2, b2);
            break;

        case 4:
            filtro_fliph(img, h1, b1, h2, b2);
            break;

        case 5:
            filtro_flipv(img, h1, b1, h2, b2);
            break;

        case 6:
            filtro_listras(img, h1, b1, h2, b2);
            break;
        }
        if(op!=0) {
            printf("Entre com o nome para salvar a imagem:\n");
            scanf("%s", &nome_img_novo);
            salvar_arquivo_ppm_p3(nome_img_novo, img);
        };
    }while (op != 0);
    free(img);
    return 0;
}
