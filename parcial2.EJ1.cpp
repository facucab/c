#include<stdio.h>
#include<string.h>
typedef struct {
	int cod;
	char desc[30];
	char gen;
	int cant;
}anio2023;
typedef struct {
	int zona;
	int sector;
	int ncasa;
	char nombre[30];
	int codJ;
} pedidos,faltantes;
//funciones:
//buscar
int buscar(anio2023 aux[], int cant, int e){
	int pos=-1,i=0;
	while(pos==-1 && i<cant){
		if(aux[i].cod == e){
			pos= i;
			return pos;
		}
		else{
			i++;
		}
	}
	return pos;
}
//carga:
void carga(anio2023 aux[]){
	anio2023 an;
	int x=0;
	FILE * ped;
	ped= fopen("ANIO2023.dat", "rb");
	fread(&an, sizeof(an),1,ped);
	while(!feof(ped)){
		aux[x]=an;
		fread(&an, sizeof(an),1,ped);
		x++;
	}
	fclose(ped);
}
//muestra
void muestra(faltantes falt){
	FILE * falta;
	falta= fopen("Pedi_not.dat","rb");
	printf("\nLISTA DE FALTANTES:");
	fread(&falt,sizeof(falt),1,falta);
	while(!feof(falta)){
		printf("\n%d\t%d\t%d\t%s\t%d",falt.zona,falt.sector,falt.ncasa,falt.nombre,falt.codJ);
		fread(&falt,sizeof(falt),1,falta);
	}
}
//GRABAR:
void grabar(faltantes fal, FILE *fa){
	fwrite(&fal,sizeof(fal),1,fa);
}
int main(){
		int acumF=0, acumM=0; // acumuladores para ver el total de pedidos masculinos y femeninos
		int sen=0, mayor=0, mayorVenta=0; // sen=> señal , mayor=> el nombre de la zona mayor, mayoventa=> la cantidad de ventas mayores de cada zona.
		float por; // sacar el porcentaje de F y M.
		anio2023 aux[200]; // instancio estructura.
		faltantes fal; // instancio estructura de faltantes.
		pedidos p; //instancio estructura de pedidos.
		//FILES:
		FILE *anio,*fa;
		//Llamo a la funcion carga para llevar al archivo anio2023 a un array de estructuras.
		carga(aux);
		//abro archivos:
		anio=fopen("pedidos.dat","rb");
		fa= fopen("Pedi_not.dat", "wb");
		//CORTE DE CONTROL:
			fread(&p,sizeof(p),1,anio); 
			while(!feof(anio)){
				int zonaAUX= p.zona; // variable auxiliar para saber la zona.
				int cont=0; // cantidad de pedidos
				int f=0, m=0; // f=> cantidad de pedidos nenas y m=> nenes
					while(!feof(anio) && zonaAUX== p.zona){ // corte de control por zona
						//ALGORITMO A)
						int pos= buscar(aux,6,p.codJ); // uso la funcion buscar para encontrar la poosicion
						if(aux[pos].gen== 'F' || aux[pos].gen== 'f'){ // comparo con F
							f++; 
						}
						else{
							m++;
						}
						//ALGORITMO C)
							aux[pos].cant= aux[pos].cant-1; 
							if(aux[pos].cant<0){
									fal= p;
									grabar(fal,fa); // funcion grabar
							}
						cont++;
						fread(&p, sizeof(p),1,anio); // lectura corte de  control
					}
					//MUESTRA PUNTO A)
					por= (float) f/cont;
					por= por*100;
					printf("\nEl porcentaje de mujeres de la zona %d es de %.1f",zonaAUX, por); // muestra la zona con la variable auxiliar zonaAUX
					por= (float) m/cont;
					por= por*100;
					printf("\nEl porcentaje de hombres de la zona %d  es de %.1f",zonaAUX, por);
					printf("\nLa cantidad de juguetes que piedieron es de %d", cont);
					//ALGORITMO B)
					if(sen==0 || cont > mayorVenta){ // Uso una señal para saber si es la primera iteracion y comparo cont con mayor venta
						sen=1;
						mayorVenta= cont; 
						mayor= zonaAUX;
					}
					//ALGORITMO E)
					acumF+= f; //acumulo todos los pedidos de nenas.
					acumM+= m; // acumulo todos los pedidos de nenes. 
					//prueba
					
		}
		fclose(fa);
			printf("\nLa sucursal %d es la que mayor ventas hizo con %d",mayor,mayorVenta);//muestro el punto b)
				//muestra punto d)
			if(acumF>acumM){ // dependiendo del mayor
				printf("\nLas nenas solicitaron mayor cantidad de juguetes, siendo %d", acumF);
			}
			else{
				printf("\nLos nenes solicitaron mayor cantidad de juguetes, siendo %d", acumM);
			}
			//muestra punto c) con funcion
			muestra(fal);
}
