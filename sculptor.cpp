#include "sculptor.hpp"
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cmath>

Sculptor::Sculptor(int _nx, int _ny, int _nz){

    nx = _nx;
    ny = _ny;
    nz = _nz;

    V = new Voxel**[nx];
    for(int i = 0; i < nx; i++) {
        V[i] = new Voxel*[ny];

        for(int j = 0; j < ny; j++) {
            V[i][j] = new Voxel[nz];

            for(int k=0; k< nz; k++) {
                V[i][j][k].show = false;
            }

        }
    }
}

Sculptor::~Sculptor(){  
  delete[] V[0][0];
  delete[] V[0];
  delete[] V;
}

void Sculptor::setColor(float r, float g, float b, float a) {
   
   this-> r = r;
   this-> g = g;
   this-> b = b;
   this-> a = a;
}

void Sculptor::putVoxel(int x, int y, int z) {
    V[x][y][z].show = true;
    V[x][y][z].r = r;
    V[x][y][z].g = g;
    V[x][y][z].b = b;
    V[x][y][z].a = a;
}

void Sculptor::cutVoxel(int x, int y, int z){
  V[x][y][z].show = false;
}

void Sculptor::putBox(int x0, int x1, int y0, int y1, int z0, int z1){
  for (int i = x0; i <= x1; i++){

    for (int j = y0; j <= y1; j++){

      for (int k = z0; k <= z1; k++){
        
        putVoxel(i, j, k);
      }
    }
  }
}
  
void Sculptor::cutBox(int x0, int x1, int y0, int y, int z0, int z1){

    for (int i = x0; i <= x1; i++){

        for (int j = y0; j <= y; j++){

            for (int k = z0; k <= z1; k++){
                cutVoxel(i, j, k);
            }
        }
    }
}

void Sculptor::putSphere(int xcenter, int ycenter, int zcenter, int radius){
    if (0 <= xcenter < nx && 0 <= ycenter < ny && 0 <= zcenter < nz){
        for (int i = 0; i < nx; i++){

            for (int j = 0; j < ny; j++){

                for (int k = 0; k < nz; k++){

                    int sphere_eq = (i - xcenter) * (i - xcenter) + (j - ycenter) * (j - ycenter) + (k - zcenter) * (k - zcenter);
                    if (sphere_eq <= (radius) * (radius)){
                        putVoxel(i, j, k);
                    }
                }
            }
        }
    }
}

void Sculptor::cutSphere(int xcenter, int ycenter, int zcenter, int radius){
    if (0 <= xcenter < nx && 0 <= ycenter < ny && 0 <= zcenter < nz)
    {
        for (int i = 0; i < nx; i++)
        {
            for (int j = 0; j < ny; j++)
            {
                for (int k = 0; k < nz; k++)
                {

                    int sphere_eq = (i - xcenter) * (i - xcenter) + (j - ycenter) * (j - ycenter) + (k - zcenter) * (k - zcenter);
                    if (sphere_eq <= (radius) * (radius))
                    {
                        V[i][j][k].show = false;
                    }
                }
            }
        }
    }
}

void Sculptor::putEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){

    if (0 <= xcenter < nx && 0 <= ycenter < ny && 0 <= zcenter < nz){
        for (int i = 0; i < nx; i++){
            for (int j = 0; j < ny; j++){
                for (int k = 0; k < nz; k++){
                    
                    int ellipsoid_eq = (((i - xcenter) * (i - xcenter)) / (rx * rx)) + (((j - ycenter) * 
                    (j - ycenter)) / (ry * ry)) + (((k - zcenter) * (k - zcenter)) / (rz * rz));
                   
                    if (ellipsoid_eq <= 1){

                        V[i][j][k].show = true;
                        V[i][j][k].r = r;
                        V[i][j][k].g = g;
                        V[i][j][k].b = b;
                        V[i][j][k].a = a;
                    }
                }
            }
        }
    }
}
void Sculptor::cutEllipsoid(int xcenter, int ycenter, int zcenter, int rx, int ry, int rz){

    if (0 <= xcenter < nx && 0 <= ycenter < ny && 0 <= zcenter < nz){

        for (int i = 0; i < nx; i++){

            for (int j = 0; j < ny; j++){

                for (int k = 0; k < nz; k++){

                    int ellipsoid_eq = (((i - xcenter) * (i - xcenter)) / (rx * rx)) + 
                    (((j - ycenter) * (j - ycenter)) / (ry * ry)) + (((k - zcenter) * (k - zcenter)) / (rz * rz));

                    if (ellipsoid_eq <= 1){

                        V[i][j][k].show = false;
                    }
                }
            }
        }
    }
}

void Sculptor::writeOFF(const char *filename)
{
  int x, y, z, indice;
  indice = 0;
  float lado = 0.5;
  int numeroVertices = 0;
  int numeroFaces = 0;

  std::ofstream arquivoGerado; // Cria um objeto do tipo ofstream para escrever

  fixed(arquivoGerado); // Define a precisão de saída

  arquivoGerado.open(filename); // Abre o arquivo para escrita

  // Verifica se foi possível abrir o arquivo, caso contrário encerra o programa
  if (arquivoGerado.is_open())
  {
    std::cout << "Arquivo aberto com sucesso" << std::endl;
  }
  else
  {
    std::cout << "Não foi possível abrir o arquivo." << std::endl;
    exit(1); // Encerra o programa com código de erro 1, EXIT_FAILURE
  }

  // Escreve OFF no arquivo
  arquivoGerado << "OFF\n";

  // Laço para identificar a quantidade de vertices e faces do voxel
  for (x = 0; x < nx; x++)
  {
    for (y = 0; y < ny; y++)
    {
      for (z = 0; z < nz; z++)
      {
        if (V[x][y][z].show)
        {
          numeroVertices += 8;
          numeroFaces += 6;
        }
      }
    }
  }

  // Escreve o número de vértices e faces no arquivo
  // x[0]  y[0]  z[0]
  arquivoGerado << numeroVertices << " " << numeroFaces
                << " 0 \n"; //   VÉRTICES, FACES, ARESTAS = 0

  // Laço que escreve as posições dos vértices
  for (x = 0; x < nx; x++)
  {
    for (y = 0; y < ny; y++)
    {
      for (z = 0; z < nz; z++)
      {
        if (V[x][y][z].show)
        {
          //  VÉRTICES DO VOXEL EM FORMATO DE CUBO
          arquivoGerado << x - lado << " " << y + lado << " " << z - lado
                        << std::endl; // Vértice 0
          arquivoGerado << x - lado << " " << y - lado << " " << z - lado
                        << std::endl; // Vértice 1
          arquivoGerado << x + lado << " " << y - lado << " " << z - lado
                        << std::endl; // Vértice 2
          arquivoGerado << x + lado << " " << y + lado << " " << z - lado
                        << std::endl; // Vértice 3
          arquivoGerado << x - lado << " " << y + lado << " " << z + lado
                        << std::endl; // Vértice 4
          arquivoGerado << x - lado << " " << y - lado << " " << z + lado
                        << std::endl; // Vértice 5
          arquivoGerado << x + lado << " " << y - lado << " " << z + lado
                        << std::endl; // Vértice 6
          arquivoGerado << x + lado << " " << y + lado << " " << z + lado
                        << std::endl; // Vértice 7
        }
      }
    }
  }

  // Escreve a conexao dos vertices para formar as faces e suas cores
  for (int x = 0; x < nx; x++)
  {
    for (int y = 0; y < ny; y++)
    {
      for (int z = 0; z < nz; z++)
      {
        if (V[x][y][z].show)
        {
          // o incide  garante que os pontos do cubo atual não se
          // sobreponham aos pontos do cubo anterior no arquivo de saída.

          arquivoGerado << 4 << ' ' << indice + 0 << ' ' << indice + 3 << ' '
                        << indice + 2 << ' ' << indice + 1 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          arquivoGerado << 4 << ' ' << indice + 4 << ' ' << indice + 5 << ' '
                        << indice + 6 << ' ' << indice + 7 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          arquivoGerado << 4 << ' ' << indice + 0 << ' ' << indice + 1 << ' '
                        << indice + 5 << ' ' << indice + 4 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          arquivoGerado << 4 << ' ' << indice + 0 << ' ' << indice + 4 << ' '
                        << indice + 7 << ' ' << indice + 3 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          arquivoGerado << 4 << ' ' << indice + 3 << ' ' << indice + 7 << ' '
                        << indice + 6 << ' ' << indice + 2 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          arquivoGerado << 4 << ' ' << indice + 1 << ' ' << indice + 2 << ' '
                        << indice + 6 << ' ' << indice + 5 << ' '
                        << V[x][y][z].r << ' ' << V[x][y][z].g << ' '
                        << V[x][y][z].b << ' ' << V[x][y][z].a << std::endl;

          indice += 8;
        }
      }
    }
  }
  // Fecha o arquivo
  arquivoGerado.close();
  std::cout << filename << " foi criado com sucesso!" << std::endl;
}