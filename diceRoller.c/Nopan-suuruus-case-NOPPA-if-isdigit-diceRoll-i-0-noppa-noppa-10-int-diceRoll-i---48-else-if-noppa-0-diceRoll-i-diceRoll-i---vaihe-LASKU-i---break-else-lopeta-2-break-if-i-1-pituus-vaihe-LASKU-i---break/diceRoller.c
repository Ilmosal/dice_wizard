#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "/home/cubanfrog/Documents/Programs/mtwist-1.5/mtwist.h"

int *throwDice(char* diceRoll)
{
        enum vaihe {OPERAATIO = 0, MAARA = 1, TYYPPI = 2, NOPPA = 3, LASKU = 4};
        enum noppa_tyyppi {NORMAALI, ONNISTUMIS};
        int vaihe = OPERAATIO;
        int maara = 0, noppa = 0, operaatio, summa = 0, kaikki_heitot = 0, lopeta = 0, nppa_tpi;
        int osien_maara = 1,i,j, pituus = strlen(diceRoll);
        int *tulokset;
        //malloc tuloksille
        tulokset = malloc(sizeof(int));
        tulokset[0]=2;
        //Go through the whole string and construct the substrings
        for (i = 0; i < pituus && lopeta == 0; i++) {
                switch(vaihe) {
                //Operaatio vaihe jossa tarkastetaan etuliite
                case OPERAATIO:
                                if (diceRoll[i+1] == 0) { lopeta = 1; break; }
                                if (diceRoll[i] == '+') { operaatio = 1;
                                } else if (i == 0) { operaatio = 1; i--;
                                } else if (diceRoll[i] == '-') { operaatio = -1;
                                } else { lopeta = 2; }
                                vaihe = MAARA;
                                break;
                //Noppien määrän tarkastus 
                case MAARA:
                                if (diceRoll[i+1] == 0 ) { vaihe = LASKU; }
                                if (diceRoll[i] == '+' || diceRoll[i] == '-') { vaihe = LASKU; i--; break;}
                                if (isdigit(diceRoll[i]) != 0) { maara = maara*10 + (int)diceRoll[i] - 48;
                                } else { vaihe = TYYPPI; i--;}
                                if (vaihe == LASKU) {i--;}
                                break;
                //Heiton tyypin tarkastus
                case TYYPPI:
                                if (diceRoll[i+1] == 0) {;lopeta = 2; break; }
                                if (maara == 0) {maara = 1;}
                                if (diceRoll[i] == 'd') { nppa_tpi = NORMAALI; vaihe = NOPPA;
                                } else {lopeta = 2; }
                                break;
                //Nopan suuruus
                case NOPPA:
                                if (isdigit(diceRoll[i]) != 0) { noppa = noppa*10 + (int)diceRoll[i] - 48;
                                }else if(noppa!=0&&(diceRoll[i] == '+'||diceRoll[i] == '-')){vaihe = LASKU; i--;break;
                                } else {lopeta = 2; break;}
                                if (i+1 == pituus) { vaihe = LASKU; i--;}
                                break;
                //lasku
                case LASKU:
                                if (nppa_tpi != NORMAALI) { lopeta = 2; break; }
                                if (noppa == 0) { summa+=operaatio*maara; vaihe=OPERAATIO; i--; maara = 0; break;}
                                tulokset[0] = tulokset[0] + maara;
                                for (j = 0; j < maara; j++) {
                                        tulokset = realloc(tulokset, (osien_maara+1)*sizeof(int));
                                        tulokset[osien_maara] = 1 + abs((int) mt_lrand() % noppa);
                                        summa+=operaatio*(tulokset[osien_maara]);
                                        osien_maara++;
                                }
                                noppa = 0, maara = 0;
                                if (i+1 < pituus) {i--; vaihe=OPERAATIO;}
                                else {lopeta = 1;}
                                break;
                }
        }
        if (lopeta == 2) {
                tulokset[0] = 0;
        } else {
                tulokset = realloc(tulokset, (osien_maara+1)*sizeof(int));
                tulokset[osien_maara] = summa;
        }

        return tulokset;
}
