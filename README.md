# Color Quantization with Clustering
Para compilar: cc *.c -o colorcluster -lm
Uso: ./colorcluster número_de_cluster arquivo_entrada arquivo_saida -r/s -rgb/lab

## Valores Iniciais dos Centroides
-r -> Os valores dos centroides são escolhidos aleatoriamente.
-s -> O usuário escolhe os valores dos centroides.

## Representação de Cores Utilizada
-rgb -> Trabalha com dados utilizando o padrão sRGB de cores.
-lab -> Trabalha com dados utilizando o padrão CIE-L*ab de cores. (ainda não implementado)
