**Que algoritmo decidimos utilizar**

Para resolver el problema de la busqueda de camino mas corto decidimos utilizar el algoritmo de djikstra debido a que algorimos como prim o kruskal debido a que nos genereraban varias limitaciones.

![](https://i.imgur.com/nWLftyu.png)


**Dijsktra Algoritmo secuencial**

Diseño de algoritmo basico para Dijsktra secuencial, para esto utilizamos el algoritmo basico teorico, y adema utilizaremos como metodo de ingreso una matriz de adyasencia que nos facilitara la paralelizacion mas basica, la vectorial.

```cpp
método Dijkstra(Grafo,origen):
    creamos una cola de prioridad Q
    agregamos origen a la cola de prioridad Q
    mientras Q no este vacío:
        sacamos un elemento de la cola Q llamado u
        si u ya fue visitado continuo sacando elementos de Q    
        marcamos como visitado u
        para cada vértice v adyacente a u en el Grafo:
            sea w el peso entre vértices ( u , v )  
            si v no ah sido visitado:
                Verificacion( u , v , w )

método Verificacion( actual , adyacente , peso ):
    si distancia[ actual ] + peso < distancia[ adyacente ]
        distancia[ adyacente ] = distancia[ actual ] + peso
        agregamos adyacente a la cola de prioridad Q
```

**Dijsktra Algoritmo Paralelo**

Para la paralelizacion del algorimo planteado con anterioridad utilizaremos OpenMP y las metologias presentadas en el MOOC de sistemas paralelos de intel, para realizar estas mejoras es necesario hacer algunos cambios en la estructura basica del codigo y utilizar las facilidades que ofrece la libreria OpenMP por medio de pragmas y mutex, ademas es necesario estructura algunas secciones de codigo para facilitar las operaciones de vectores por medio de vectorizacion.
```cpp
método Dijkstra(Grafo,origen):
    creamos una cola de prioridad Q
    agregamos origen a la cola de prioridad Q
    **Paralelizar el for (omp for)**
    para cada Q no este vacío:
        **Paralelizacion de vector (simd)**
        **Aplicar Race Mutex (atomic)**  
        sacamos un elemento de la cola Q llamado u
        si u ya fue visitado continuo sacando elementos de Q 
        marcamos como visitado u
        **paralelizar el for*
        para cada vértice v adyacente a u en el Grafo:
            **Aplicar Race Mutex (critical)**  
            sea w el peso entre vértices ( u , v )  
            si v no ah sido visitado:
                Verificacion( u , v , w )
**Paralelizacion de vector (simd)**
método Verificacion( actual , adyacente , peso ):
    si distancia[ actual ] + peso < distancia[ adyacente ]
        distancia[ adyacente ] = distancia[ actual ] + peso
        agregamos adyacente a la cola de prioridad Q
```


