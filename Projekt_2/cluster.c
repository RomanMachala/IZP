/**
 * Kostra programu pro 2. projekt IZP 2022/23
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    //Alokace paměti pro cluster o kapacitě 1 (int cap = 1, 1 objekt = 1 cluster).
    c->obj=malloc(sizeof(struct obj_t)*cap);
    if (c->obj==NULL)
    {
        c->capacity=0;
        return;
    }
    //Cluster nemá žádný objekt, proto size=0.
    c->size=0;
    //Kapacita = 1.
    c->capacity=cap;

}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    //Uvolnime c->obj a natavíme na NULL.
    if (c->obj!=NULL)
    {
        free(c->obj);
        c->obj=NULL;
        
    }
    //Shluk nemá žádný objekt, proto cap a size=0.
    c->capacity=0;
    c->size=0; 
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    //Pokud máme dostatečnou kapacitu, není třeba realokovat. Vracíme c zpět.
    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    //Realokace místa, abychom měli dostatečnou kapacitu.
    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    //size=capacity -> ve shluku není místo pro další prvek.
    if (c->size == c->capacity)
    {
        //Realokace místa pro vložení nového prvku.
        if (resize_cluster(c, c->capacity+CLUSTER_CHUNK)==NULL)
        {
            return;
        }
    }
    if (c->obj == NULL)
    {
        return;
    }
    if (c->size < c->capacity)
    {
        //Vložíme prvek na pozici c->size a zvýšíme size shluku o 1.
        c->obj[c->size]=obj;
        c->size++;
    }
    // TODO
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    //c1 a c2 jsou pozice shluků, které jsou k sobě nejblíže.
    assert(c1 != NULL);
    assert(c2 != NULL);
    //Alokace místa pro větší shluk (Celková velikost= size1 + size2).
    if (resize_cluster(c1, c1->size + c2->size)==NULL)
    {
        return;
    }
    for (int i=0; i<c2->size; i++)
    {
        //Přidáváme prvek za poslední prvek ve shluku c1.
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
    // TODO
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    //narr=počet shluků
    //idx=shluk[idx], který chceme odstranit.
    assert(idx < narr);
    assert(narr > 0);
    //Odstraníme shluk na pozici idx.
    clear_cluster(&carr[idx]);
    //Přesuneme postupně jeden shluk za druhým o pozici dopředu(Začínáme od indexu, kde jsme shluk odstranili).
    for (int i=idx; i<(narr-1); i++)
    {
        carr[i]=carr[i+1];
    }
    int new_narr=narr-1;
    //Vracíme nový počet shluků.
    return new_narr;
    // TODO
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    float par_x=o1->x-o2->x;
    float par_y=o1->y-o2->y;
    //Euklidovská vzdálenost 2 bodů.
    float euklidean_distance=sqrt((par_x * par_x)+(par_y * par_y));

    return euklidean_distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    //Počítám vzdálenost objektů v jednom shluku vůči objektům druhého shluku.
    float distance, temp_distance=0;
    for (int i=0; i<c1->size; i++)
    {
        for (int n=0; n<c2->size; n++)
        {
            //Vzdálenost počítám každého prvku s každým a ukládám do temp_distance.
            temp_distance=obj_distance(&c1->obj[i], &c2->obj[n]);
            {   
                //První vypočtenou vzdálenost uložím do distance, pak přepisuji hodnotu v distance, pouze pokud nová vypočtená vzdálenost bude menší.
                if ((i==0 && n==0) || temp_distance<distance)
                {
                    distance=temp_distance;
                }
            }
        }
    }
    //Vracím nejmenší vzdálenost (vzdálenost dvou shluků = nejmenší vzdálenost 2 bodů z těchto shluků).
    return distance;
    // TODO
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    
    float distance=INFINITY;
    float temp_distance=0;
    //Počítám vzdálenosti všech shluků vůči všem.
    for (int i=0; i<narr; i++)
    {
        for (int n=0; n<narr; n++)
        {
            //Přeskakuji, protože vzdálenost shluku od sebe sama je 0.
            if (i==n)
            {
                continue;
            }
            //Vypočítám vzdálenost 2 shluků.
            temp_distance=cluster_distance(&carr[i], &carr[n]);
            if (temp_distance<distance)
            {
                //Pokud nová vypočtená vzdálenost < předchozí === uložíme to jako novou distance.
                distance=temp_distance;
                //Ukládám si pozici dvou shluků do c1 a c2.
                *c1=i;
                *c2=n;
            }
        }
    }
    // TODO
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr, int *loaded_objects)
{
    
    FILE *fptr;
    fptr=fopen(filename, "r");
    //Kontrola, zda soubor byl otevřen.
    if (fptr==NULL)
    {
        return -1;
    }

    //Načtení prvního řádku, odkud získáme počet načítaných objektů.
    int objects_quantity=0;
    //Kontrola, že načteme jedno číslo ze vstupu, pokud ne, vracíme chybu a zavíráme soubor.
    if (fscanf(fptr, "count=%d\n", &objects_quantity)!=1)
    {
        fclose(fptr);
        return -2;
    }

    *arr=malloc(objects_quantity*sizeof(struct cluster_t));
    if (*arr==NULL)
    {
        fclose(fptr);
        return -3;
    }
    //Pro případ, že bude třeba dealokovat místo při nevalidním vstupu.
    *loaded_objects=objects_quantity;
    struct obj_t object={0, 0.0, 0.0};

    int i=0;
    //objects_quantity je počet načítaných objektů udán v souboru>i a fscanf nám musí načíst 3 hodnoty, aby podmínka byla TRUE.
    while ((i<objects_quantity) && (fscanf(fptr, "%d %f %f\n", &object.id, &object.x, &object.y)==3))
    {
        init_cluster(&arr[0][i], 1);
        append_cluster(&arr[0][i], object);
        i++;
    }
    //Počet načtených objektů pomocí fcanf.
    int count=i;

    //Pokud počet ze souboru > počet načtených objektů.
    if (objects_quantity>count)
    {
        fclose(fptr);
        return -4;
    }

    int invalid_cords=0;
    int invalid_id=0;

    //Kontrola, že objekty (které byly načteny) mají x a y > 0 && < 1 000 a že dva objekty (nebo více) nemají stejné id.
    for (int s=0; s<count; s++)
    {
        if (arr[0][s].obj->x > 1000 || arr[0][s].obj->y > 1000 || arr[0][s].obj->x < 0 || arr[0][s].obj->y < 0)
        {
            invalid_cords++;
        }
        for (int t=1; (t+s)<count; t++)
        {
           if (arr[0][s].obj->id==arr[0][s+t].obj->id)
           {
                invalid_id++;
           } 
        }
    }

    if (invalid_cords!=0 || invalid_id!=0)
    {
        fclose(fptr);
        return -5;
    }

    assert(arr != NULL);
    fclose(fptr);
    //Vracíme počet načtených objektů.
    return count;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

//quantity=návratová hodnota z load_clusters, object_count=count z file
int input_validation(int quantiny, int object_count, int loaded_objects, struct cluster_t *c)
{
    
    if (quantiny ==-1)
    {
        //Soubor nebyl nalezen, nebo nemáme práva na jeho otevření.
        fprintf (stderr, "Error: File not found.\n");
        return -1;
    }
    else if (quantiny ==-2)
    {
        //Pokud nenajdeme v souboru počet objektů.
        fprintf (stderr, "Error: Count of objects not specified.\n");
        return -2;
    } 
    else if (quantiny ==-3)
    {
        //Pokud se nám nepovede alokovat paměť.
        fprintf (stderr, "Error: Memory allocation error.\n");
        return -3;
    }
    else if (quantiny ==-4)
    {
        //Pokud count v souboru > počet načtených objektů.
        fprintf (stderr, "Error: Object count is higher than loaded objects.\n");
        for (int i=0; i<loaded_objects; i++)
        {
            clear_cluster(&c[i]);
        }
        return -4;
    }
    else if (quantiny ==-5)
    {
        //Soubor obsahuje objekt s nevalidními souřadnicemi x || y nebo minimálně dva objekty mají stejné id.
        fprintf (stderr, "Error: Objects are invalid.\n");
        for (int i=0; i<loaded_objects; i++)
        {
            clear_cluster(&c[i]);
        }
        return -5;
    }
    else if (object_count>quantiny)
    {
        //Pokud počet požadovaných clusterů je větší, než počet načtených objektů.
        fprintf(stderr, "Error: Cluster count cannot be higher than object count.\n");
        for (int i=0; i<loaded_objects; i++)
        {
            clear_cluster(&c[i]);
        }
        return -6;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
    //tempCount je počet clusterů, nastavený na 1 (hodnota při absenci počtu clusterů v argumentu).
    int tempCount=1;
    //Loaded objects=počet načtených objektů pro uvolnění shluků v případě invalidního vstupu.

    int loaded_objects=0;

    if (argc==1 || argc>3)
    {
        fprintf(stderr,"Error: Not enough or too many arguments.\n");
        return 1;
    }
    if (argc == 3)
    {   
        //Převod argumentu ze stringu na integer.
        int argument=0;
        for (int i=0; argv[2][i]!='\0'; i++)
        {
            argument=(10*argument)+(argv[2][i]-'0');
        }
        //Uložíme požadovaný počet clsuterů do tempCount.
        tempCount=argument;
    }
   
    
    
    int quantity_of_objects=load_clusters(argv[1], &clusters, &loaded_objects);
    //Zkontrolujeme vstupy na validaci (soubor, počet clusterů, soubor ve správném formátu, objekty ve správném formátu, ...).
    int validation=input_validation(quantity_of_objects, tempCount, loaded_objects, clusters);
    //Ukončení, pokud vstupy nejsou validní.
    if (validation!=1)
    {   
        free(clusters);
        return 1;
    }

    //quantity_of_objects=počet načtených objektů=počet vytvořených clusterů (ze začátku má každý objekt svůj cluster).
    //tempCount=požadovaný počet clusterů.
    while (quantity_of_objects>tempCount)
    {
        //Podmínka bude true do doby, dokud se nedosáhne požadovaného počtu clusterů.

        //Proměnné, do kterých budeme ukládat pozice 2 nejbližších clusterů.
        int position1=0;
        int position2=0;
        //Najdeme nejbližší clustery a jejich pozice ukládáme do position1 a position2.
        find_neighbours(clusters, quantity_of_objects, &position1, &position2);
        //Spojujeme dva clustery, které mají k sobě nejblíže.
        merge_clusters(&clusters[position1], &clusters[position2]);
        //Odstraníme cluster, který je na pozici cluster2.

        quantity_of_objects=remove_cluster(clusters, quantity_of_objects, position2);
        //Odstraníme shluk[position2] z pole shluků (clusters) které má quantiny_of_objects shluků.
        //Dostáváme zpět nový počet shluků (o 1 méně).
    }
    print_clusters(clusters, tempCount);

    //Dealokace clusterů (tempCount=počet výsledných clusterů).
    for (int i=0; i<tempCount; i++)
    {
        clear_cluster(&clusters[i]);
    }
    //Uvolnění samotného pole clusterů.
    free(clusters);
    return 0;
    // TODO
}