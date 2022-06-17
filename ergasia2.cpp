#include "AAtree.h"
#include <functional>
#include <iostream>
#include <random>

using std::cout, std::cin, std::endl, std::bind;

void findNearestLootOnX(AATree<int> &t, int thesi[], int N);

int main()
{
    // O xrhsths orizei th diastash toy kyboy kai tis kinhseis poy ua ginoyn apo ton hrwa
    int N, moves;
    cout << "Dwste th diastash toy kyboy: ";
    cin >> N;
    cout << endl
         << "Dwste tis kinhseis poy ua kanei o hrwas: ";
    cin >> moves;
    cout << endl;

    // Gennhtria tyxaiwn arithmwn apo to 1 mexri to 100 gia th leia twn keliwn
    default_random_engine generator;
    uniform_int_distribution<int> loot_element_distribution(0, 100);
    auto random_loot = bind(loot_element_distribution, generator);

    // Gennhtria tyxaiwn keliwn gia topothethsh leias
    uniform_int_distribution<int> cell_element_distribution(0, N);
    auto random_cell = bind(cell_element_distribution, generator);

    // Gennhtria tyxaiwn arithmwn gia metatophsh toy hrwa
    uniform_int_distribution<int> move_element_distribution(-1, 1);
    auto random_move = bind(move_element_distribution, generator);

    // Dhmioyrgia toy kyboy
    AATree<int> kybos;
    int loot = 0; // Arxikopoihsh leias toy hrwa

    // Prosthikh N mh kenwn stoixeiwn ston kybo
    int cell[3];
    for (int i = 0; i < N; i++)
    {
        for (int i = 0; i < 3; i++)
            cell[i] = random_cell();
        kybos.insert(cell, random_loot());
    }

    // Topothetish toy hrwa se ena tyxaio keli
    int thesi_hrwa[3] = {random_cell(), random_cell(), random_cell()};

    // Kinhsh hrwa
    for (int i = 0; i < moves; i++)
    {
        for (int i = 0; i < 3; i++)
        {
            int move = random_move();
            // Elegxos ean to keli einai egkyro kai den exei bgei ektos kyboy
            while (thesi_hrwa[i] + move < 0 || thesi_hrwa[i] + move > N)
                move = random_move();
            thesi_hrwa[i] += move;
        }

        if (kybos.find(thesi_hrwa))
        {
            loot += kybos.find(thesi_hrwa); // O hrwas karpwnetai thn leia toy kelioy
            kybos.remove(thesi_hrwa);       // Mhdenizoyme th leia toy sigkekrimenoy kelioy
            for (int i = 0; i < 3; i++)
                cell[i] = random_cell();
            kybos.insert(cell, random_loot()); // Prosthetoyme leia tyxaiwn pontwn se tyxaio keli
        }
    }
    cout << "H leia poy mazepse o hrwas einai: " << loot << endl;

    // Ektelesh ths diadikasias poy perigrafetai sthn ekfwwnhsh
    findNearestLootOnX(kybos, thesi_hrwa, N);

    return 0;
}

// Prostheth dynatothta poy zhteitai
void findNearestLootOnX(AATree<int> &t, int thesi[], int N)
{
    int found1 = -1, found2 = -1, distance1 = 0, distance2 = 0, arxikh_thesi = thesi[0];
    // Elegxoyme panw apo to keli
    while (thesi[0] < N)
    {
        thesi[0]++;
        if (t.find(thesi))
        {
            found1 = thesi[0];
            distance1 = thesi[0] - arxikh_thesi;
            break;
        }
    }
    thesi[0] = arxikh_thesi;

    // Elegxoyme katw apo to keli
    while (thesi[0] > 0)
    {
        thesi[0]--;
        if (t.find(thesi))
        {
            found2 = thesi[0];
            distance2 = arxikh_thesi - thesi[0];
            break;
        }
    }

    if (found1 == -1 && found2 == -1) // Den brethike keli me leia ston aksona x
        cout << "Den brethike keli sthn kateythinsh toy aksona x poy na periexei leia." << endl;
    else if (distance1 != distance2) // To ena keli einai pio konta apo to allo
        cout << "To plhsiestero keli sth kateythinsh toy aksona x poy periexei leia einai to: [" << ((distance2 = 0 || distance1 < distance2) ? found2 : found1) << ", " << thesi[1] << ", " << thesi[2] << "]." << endl;
    else // Brethikan 2 kelia poy isapexoyn apo th thesi toy hrwa
        cout << "Ta dyo plhsiestera kelia sth kateythinsh toy aksona x poy periexoyn leia einai ta: [" << found1 << ", " << thesi[1] << ", " << thesi[2] << "], [" << found2 << ", " << thesi[1] << ", " << thesi[2] << "].";
}