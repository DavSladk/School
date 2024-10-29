/**
 * VUT FIT IMS project
 * Uhlikova stopa v energetice a teplarenstvi
 * @author xsladk08 David Sladky
 * @author xandrl09 Ondřej Andrla
 */

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>
#include "simlib.h"



using namespace std;


unsigned int years = 0; // time of simulation
unsigned int builded_coal = 0; // amount of coal power plants
unsigned int wind = 0; // amount of planned wind turbines


long long int money = 0;
unsigned long int elektricity = 0;
unsigned long int emissions = 0;

unsigned long int wind_elektricity = 0;
unsigned long int wind_emissions = 0;
long long int wind_money = 0;


void bad_args()
{
    cout << "Wrong parametrs!" << endl << endl;

    cout << "To set time of simulation use -> -y years" << endl;
    cout << "Example: -y 50" << endl;

    cout << "To set initial amount of coal power plants use -> -b amount" << endl;
    cout << "Example: -b 1" << endl;

    cout << "To set amount of planned wind turbines use -> -w amount" << endl;
    cout << "Example: -w 20" << endl;

    exit(1);
}

void  parse_params(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt (argc, argv, "ybw")) != -1)
    {
        switch (opt)
        {
            case 'y':
                if (argv[optind] != nullptr ) {
                    years = strtol(argv[optind],nullptr,0);
                }
                else{
                    bad_args();
                }
                break;
            case 'b':
                if (argv[optind] != nullptr ) {
                    builded_coal = strtol(argv[optind],nullptr,0);
                }
                else{
                    bad_args();
                }
                break;
            case 'w':
                if (argv[optind] != nullptr ) {
                    wind = strtol(argv[optind],nullptr,0);
                }
                else{
                    bad_args();
                }
                break;
            default:
                bad_args();
        }
    }

}


int coal_power_plant()
{
    double d_time_to_live = Uniform(1, 50);
    int time_to_live = (int)round(d_time_to_live);
    cout << endl;
    cout << time_to_live << "Coal power plant TTL" << endl << endl;
    int el = 0;
    for(int i = 0; i < time_to_live; i++)
    {
        if(i == years )
        {
            break;
        }

        long double d_e = Uniform(1404 , 1604);
        unsigned int e = (int)round(d_e);

        el = el + e;


    }

    return el;
}


void coal_work() {

    for(int i = 0; i < builded_coal; i++)
    {
        unsigned int el = coal_power_plant();

        elektricity = elektricity + el;

    }

    money = elektricity * Uniform(400, 500); /// 1200 - 750; +-50 zaokrouhleni

    double d_emissions = elektricity * Uniform(760, 900);
    emissions = emissions + (int)round(d_emissions);

}


int wind_power_plant(int time_to_end)
{
    double d_time_to_live = Uniform(18, 25);
    int time_to_live = (int)round(d_time_to_live);
    //cout << time_to_live << endl << endl;
    int el = 0;
    for(int i = 0; i < time_to_live; i++)
    {
        if(i == time_to_end - 1)
        {
            break;
        }

        double years_hours = 24 * 365;
        double productive_hours = years_hours * 0.26; /// prumerna rocni vyuzitelnost
        double power_plant_production = productive_hours * 0.03;/// vetrna farma = 30 MW
        double d_e = Normal(power_plant_production, 1);
        unsigned int e = (int)round(d_e);
        el = el + e;

        money = money - 40000;
    }

    return el;
}

void wind_work() {

    for(int i = 0; i < years; i++)
    {
        if(i == wind )
        {
            break;
        }

        unsigned int el = wind_power_plant(years - i);

        elektricity = elektricity + el;

    }

    wind_elektricity = elektricity;

    double d_money = elektricity * Uniform(1150, 1250);
    money = money + (int)round(d_money);

    money = money - Uniform(1050000, 1200000) * wind;
    wind_money = money;


//    double d_emissions = elektricity * Uniform(10, 20);
//    emissions = (int)round(d_emissions);
    emissions = 241850 * wind;

    wind_emissions = emissions;

}


int main(int argc, char *argv[]) {
    parse_params(argc, argv);
    RandomSeed(time(NULL));
    wind_work();
    coal_work();

    cout << "Total amount of produced electricity = " << elektricity << "GWh" << endl;
    cout << "Total profit = " << money  << " thousand CZK" << endl;
    cout << "Total amount of CO2 = " << emissions  << "kg" << endl << endl;

    cout << "Total amount of electricity produced by wind turbines = "
         << wind_elektricity << "GWh"<< endl;
    cout << "Total profit of wind turbines = "
         << wind_money << " tisíc Kc" << endl;
    cout << "Total CO2 produced by wind turbines = "
         <<wind_emissions << "kg" << endl <<endl;

    cout << "Total amount of electricity produced by coal power plants = "
    << elektricity - wind_elektricity << "GWh"<< endl;
    cout << "Total profit of coal power plants = "
    << money - wind_money << " thousand CZK" << endl;
    cout << "Total CO2 produced by coal power plants = "
    << emissions - wind_emissions << "kg" << endl;

    return 0;
}