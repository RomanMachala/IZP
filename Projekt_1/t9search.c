/*
*Vytvořil_Machala_Roman_xmacha86
*Program funguje na principu převodu načteného jména na znaky číslic, které dané písmeno charakterizuje a následně porovnává vložený argument na shodu buďto ve jméně (převedeném na číslo) nebo v samotném číslu.
*Bližší detaily funkčnosti samotných funkcí jsou v těle daných funkcí.
*/
#include<stdio.h>
#include<string.h>

#define EXIT_SUCCESFULL 0           //Návratová hodnota pro ukončení programu.
#define EXIT_FAILURE 1              //Návratová hodnota pro ukončení programu pro případ zadání neočekávaných vstupů.
#define INPUT_UNSPECIFIED 5         //Návratová hodnota v případě, že uživatel nezadá žádný argument.
#define INPUT_FAILURE 6             //Návratová hodnota v případě, že bude zadána špatná hodnota (např. argument nebude číslo, nebo bude obsahovat nečíselné znaky, ...).
#define INPUT_SUCCESFULL 7          //Návratová hodnota pro vstupy, které byly zadány srpávně (např. argument programu obsahuje pouze čísla a nic jiného, ...).


int osetreniVstupuArgumentu (int argc)
{   
    /*
    *Funkce kontroluje počet zadaných argumentů (požadovaný počet argumentů (i s názvem programu) je 2).
    *Pokud je zadáno méně jak 2 argumenty (tedy žádný), funkce vrátí INPUT_UNSPECIFIED a vypíše všechny kontakty, které odpovídají daným kritériím (např. telefonní číslo neobsahuje jiné znaky, než jsou čísla).
    *Pokud je zadánoo více jak 2 argumenty, funkce vrátí INPUT_FAILURE a ukončí se program.
    */

    if (argc<2)
    {                                   
        return INPUT_UNSPECIFIED;     
    } 

    if (argc>2)
    {
        return INPUT_FAILURE;
    }

    return EXIT_SUCCESFULL;
}

int validaceZadanychCisel(char *name, char *number)
{
    /*
    *Funkce validaceZadanychCisel porovnává jednotlivé znaky z načteného řádku a vyhodnocuje, zda-li zadaný znak je číslo nebo mezera nebo znaménko '+'.
    *Pokud znaménko '+' je nalezeno při validaci čísla, program se ptá, zda-li toto znaménko je na první pozici. Pokud ano, je číslo uznáno za validní, pokud ne, je funkce ukončena hodnotou INPUT_FAILURE.
    *Dále se u této funkce vyhodnocuje počet načtených znaků vyhodnocených jako číslo a porovnává se to s celkovou délkou načteného řádku.
    *Pokud celková délka načteného řádku je shodná s počtem validních znaků, zanmená to, že v načteném řádku se vyskytují pouze znaky platná pro čísla (i znaménko '+', tedy pokud je pouze na první pozici).
    *Při uznání validního čísla funkce vrací hodnotu INPUT_SUCCESFULL.
    */
int nameLenght=(strlen(name));
int numberLenght=(strlen(number));

if ((nameLenght <=100) && (numberLenght <=100))
    {
        int pocetZnakuVCisle=0;
        for (int i=0; number [i] != '\0'; i++)
        {
            if ((number [i] >= '0' && number [i] <= '9') || (number[i]=='+') || (number [i]==' '))      
            {
                pocetZnakuVCisle++;
                if (number [i]=='+')
                {
                    if(i!=0)
                    {
                        return INPUT_FAILURE;
                    }
                }
            }
        }
        if (numberLenght==pocetZnakuVCisle)
        {
            return INPUT_SUCCESFULL;
        }
    }

    return EXIT_SUCCESFULL;
}

char* prevodCisla(char *number, char *numberTrans)
//Jendoduchá funkce pro zajištění převodu čísla, pokud by načtené číslo obsahovalo znaménko '+', aby bylo převedeno na znak 0. Validace samotného čísla se provádí ve funkci validaceZadanychCisel.
{
    for (int i=0; number[i]!='\0'; i++)
    {
       numberTrans[i]=number[i];
       if (number[i]=='+')
       {
        numberTrans[i]='0';
       }
    }
    return numberTrans;
}

void vypisKontaktuProZadnyArgument()
{
    /*
    *Funkce pro výpis kontaktů v případě, že uživatel nezadá argument.
    *Načítají se dva řádky ze souboru (jeden pro jméno, druhý pro číslo). Funkce while načítá další řádky, dokud jeden z řádků není prázdný řádek.
    *Načítáme vždy dva řádky, protože ke kontaktu musíme mít jméno a číslo, pokud jeden z nich chybí, nebo již není další řádek na načtení, funkce daný kontakt bere, jako by neexistoval.
    *Funkce validaceZadanýchCisel vyhodnociuji zda-li zadané čísla jsou validní (např. pokud číslo obsahuje písmeno, tak není validní). 
    */

    char jmeno [100];
    char cislo [100];
    char *porovnaniJmeno;
    char *porovnaniCislo;
    
    while (((porovnaniJmeno=gets(jmeno))!=NULL) && ((porovnaniCislo=gets(cislo))!=NULL))
    {
       if(((validaceZadanychCisel(jmeno, cislo))==INPUT_SUCCESFULL))
       {
        fprintf(stdout, "%s, %s\n", jmeno, cislo);
       }
    }  
}

char* premenaJmenaNaCislo(char *name, char *nameInNumbers)
{
for (int i=0; name [i]!='\0'; i++)
{
    /*
    *Funkce pro převod jména na číselné znaky dle numerické klávesnice (např. znaky 'a' 'b' 'c' odpovídají číslu 2).
    *Funkce se nejprve ptá, zdali načtený znak je písmeno, pokud ano, dále se ptá, o jaké písmeno se jedná a převádí jej na jeho opdovídající číselnou hodnotu.
    *Protože jména mohou obsahovat i jiné znaky než jsou písmena, které nemají svou reprezentaci jako písmena (např. znak mezery, '.', ...), funkce else zadává tyto znaky dále beze změny.
    *Funkce nám vrací jméno v jeho číselné podobě pro jeho následné porovnání se vstupním argumentem uživatele.
    */
    
    if (((name [i] >= 'A') && (name [i] <= 'Z')) || ((name [i] >= 'a') && (name [i] <= 'z')) || (name[i]=='+'))
    {
        if (((name [i] >= 'A') && (name [i] <= 'C')) || ((name [i] >= 'a') && (name [i] <= 'c')))
        {
            nameInNumbers [i]= '2';
        }
        if (((name [i] >= 'D') && (name [i] <= 'F')) || ((name [i] >= 'd') && (name [i] <= 'f')))
        {
            nameInNumbers [i]= '3';
        }
        if (((name [i] >= 'G') && (name [i] <= 'I')) || ((name [i] >= 'g') && (name [i] <= 'i')))
        {
            nameInNumbers [i]= '4';
        }
        if (((name [i] >= 'J') && (name [i] <= 'L')) || ((name [i] >= 'j') && (name [i] <= 'l')))
        {
            nameInNumbers [i]= '5';
        }
        if (((name [i] >= 'M') && (name [i] <= 'O')) || ((name [i] >= 'm') && (name [i] <= 'o')))
        {
            nameInNumbers [i]= '6';
        }
        if (((name [i] >= 'P') && (name [i] <= 'S')) || ((name [i] >= 'p') && (name [i] <= 's')))
        {
            nameInNumbers [i]= '7';
        }
        if (((name [i] >= 'T') && (name [i] <= 'V')) || ((name [i] >= 't') && (name [i] <= 'v')))
        {
            nameInNumbers [i]= '8';
        }
        if (((name [i] >= 'W') && (name [i] <= 'Z')) || ((name [i] >= 'w') && (name [i] <= 'z')))
        {
            nameInNumbers [i]= '9';
        }
        if (name [i]== '+')
        {
            nameInNumbers [i]= '0';
        }
    }
    else
    {
        nameInNumbers [i]=name [i];
    }
}
return nameInNumbers;
}

void kontaktyNenalezeny(int pocetNalezenychKontatku)
{
    //Jednoduchá funkce pro výpis "Not found." v případě, že žádný kontakt neodpovídá hledanému parametru.

    if (pocetNalezenychKontatku==0)
    {
        fprintf(stdout, "Not found\n");
    }
}

int kontrolaArgumentuNaCislo(char *argument, char** argv )
{
    /*
    *Funkce pro kontrolu, zda-li zadaný argument obsahuje pouze znaky číslic.
    *V případě, že znak obsahuje jiné znaky (např. písmeno, ...) funkce nám vrací INPUT_FAILURE pro znázornění špatně zadaného argumentu a vypisuje odpovídající error.
    *V případě, že uživatel chce vyhledávat číslice s předvolbou (např. +420, ..), argument musí začínat znaménkem '+', pokud bude na jiné pozici, program se ukončí INPUT_FAILURE a vypíše odpovídající chybové hlášení. 
    *Pokud zadaný argument vyhovuje zadaným kritériím (tedy obsahuje pouze znaky číslic a nic jiného), funkce nám vrací hodnotu INPUT_SUCCESFULL a program pokračuje dále na hledání souvisejících kontaktů.
    */

    for (int i=0;argv [1] [i]!='\0'; i++)
    {
        if (((argv [1] [i] >= '0') && (argv [1] [i] <= '9')) || (argv [1] [i]=='+'))
        {
            if (argv [1] [i]=='+')
            {
                if (i != 0)
                {
                    fprintf(stderr, "Znamenko '+' muze byt pouze na prvnim miste!\n");
                    return INPUT_FAILURE;
                }
            }
            argument [i]=argv[1] [i];
        }
        else
        {
            fprintf(stderr, "Argument muze byt pouze cislo nebo cislo zacinajici znamenkem '+' pro vyhledavani cisel s predvolbou!\n");
            return INPUT_FAILURE;
        }  
    }
return INPUT_SUCCESFULL;
}

int main(int argc, char *argv [])
{
    /*
    *Funkce osetreniVstupuArgumentu vyhodnocuje, zda-li byl argument zadán či nikoli. 
    *V případě, že bylo zadáno více, než je požadované množství argumentů nás progam informuje řádným výstupem a program končí hodnotou EXIT_FAILURE.
    *V opačném případě, že bylo zadáno nedostatečné množství argumentů dojde k vypsání všech validních kontatků a program se ukončuje hodnotou EXIT_SUCCESFULL.
    */

    if (osetreniVstupuArgumentu (argc)==INPUT_FAILURE)              //Pokud je počet zadaných argumentů vyšší než 2.
    {
        fprintf (stderr, "Zadal jste prilis mnoho argumentu!\n");
        return EXIT_FAILURE;
    }
    if (osetreniVstupuArgumentu(argc)==INPUT_UNSPECIFIED)           //Pokud nebyl zadaný žádný argument.
    {
        vypisKontaktuProZadnyArgument();                            //Funkce pro výpis kontaktů při nezadání argumentu.
        return EXIT_SUCCESFULL;
    }

    char argument [100];

    int vystupKontrolyArgumentuNaCislo=(kontrolaArgumentuNaCislo(argument, argv));    

    /*
    *Kontrola argumentu na číslo. Pokud argument obsahuje pouze číselné znaky nebo znaménko '+' na prvním místě,
    *(může být i v kombinaci s dalšími číselnými znaky), program pokračuje dále.
    *Pokud návratová hodnota funkce na validaci argumentu je INPUT_FAILURE, program je ukončen návratovou hodnotou EXIT_FAILURE.
    */

    if (vystupKontrolyArgumentuNaCislo==INPUT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    if (vystupKontrolyArgumentuNaCislo==INPUT_SUCCESFULL)
    { 
        char name [100];
        char number [100];
        char *comparisonName;
        char *comparisonNumber;
        int pocetNalezenychKontaktu=0;
        char numberTrans[100];
    
        while (((comparisonName=gets(name))!=NULL) && ((comparisonNumber=gets(number))!=NULL))          //Validace,že se opravdu načetly 2 řádky.
        {   
            prevodCisla(number, numberTrans); 

            if ((validaceZadanychCisel(name, number)==INPUT_SUCCESFULL))  //Využití funkce pro validaci čísla (že se opravdu jedná o číslo) 
            {                                                                                                                   
                char nameInNumbers [100];
                if ((strlen (name) <=100) && (strlen (number) <=100))
                {
                    premenaJmenaNaCislo(name, nameInNumbers);       //Přeměna jména na číslo pro následné porovnání na shodu s argumentem vloženým uživatelem.
                }
                
                if ((strstr(nameInNumbers, argument)!=NULL) || (strstr(numberTrans, argument)!=NULL) || (strstr(number, argument)!=NULL))        
                {
                    //Kontrola, zda-li načtené jméno či číslo obsahují posloupnost znaků určených uživatelem. numberTrans je zde v případě, že hledáme čísla s předvolbou pomocí argumentu začínající '0'.
                    pocetNalezenychKontaktu++;
                    if (pocetNalezenychKontaktu==1)                     
                    {
                        fprintf(stdout, "Kontakt(y) nalezen(y)\n");     //Výpis Kontakt(y) nalezen(y) se děje pouze při naleznutí první shody.
                    }

                    fprintf(stdout, "%s, %s\n", name, number);          //Samotný výpis kontaktů odpovídající shodě.
                }
            }
        }  

        //Funkce pro vypsání "Not found." v případě, že nebude nalezen žádný kontakt (kontakty nebudou odpovídat požadovanému hledání).
        kontaktyNenalezeny(pocetNalezenychKontaktu); 
    }

    return EXIT_SUCCESFULL;
}
