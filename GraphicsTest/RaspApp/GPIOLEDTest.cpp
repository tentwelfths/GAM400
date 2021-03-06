#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <signal.h>

#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H
using namespace std;
/* GPIO Class
 * Purpose: Each object instantiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 */
class GPIOClass
{
public:
    GPIOClass();  // create a GPIO object that controls GPIO4 (default
    GPIOClass(string x); // create a GPIO object that controls GPIOx, where x is passed to this constructor
    int export_gpio(); // exports GPIO
    int unexport_gpio(); // unexport GPIO
    int setdir_gpio(string dir); // Set GPIO Direction
    int setval_gpio(string val); // Set GPIO Value (putput pins)
    int getval_gpio(string& val); // Get GPIO Value (input/ output pins)
    string get_gpionum(); // return the GPIO number associated with the instance of an object
private:
    string gpionum; // GPIO number associated with the instance of an object
};

#endif


GPIOClass::GPIOClass()
{
    this->gpionum = "4"; //GPIO4 is default
}

GPIOClass::GPIOClass(string gnum)
{
    this->gpionum = gnum;  //Instatiate GPIOClass object for GPIO pin number "gnum"
}

int GPIOClass::export_gpio()
{
    string export_str = "/sys/class/gpio/export";
    ofstream exportgpio(export_str.c_str()); // Open "export" file. Convert C++ string to C string. Required for all Linux pathnames
    if (exportgpio < 0){
        cout << " OPERATION FAILED: Unable to export GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }

    exportgpio << this->gpionum ; //write GPIO number to export
    exportgpio.close(); //close export file
    return 0;
}

int GPIOClass::unexport_gpio()
{
    string unexport_str = "/sys/class/gpio/unexport";
    ofstream unexportgpio(unexport_str.c_str()); //Open unexport file
    if (unexportgpio < 0){
        cout << " OPERATION FAILED: Unable to unexport GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
    }

    unexportgpio << this->gpionum ; //write GPIO number to unexport
    unexportgpio.close(); //close unexport file
    return 0;
}

int GPIOClass::setdir_gpio(string dir)
{

    string setdir_str ="/sys/class/gpio/gpio" + this->gpionum + "/direction";
    ofstream setdirgpio(setdir_str.c_str()); // open direction file for gpio
        if (setdirgpio < 0){
            cout << " OPERATION FAILED: Unable to set direction of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }

        setdirgpio << dir; //write direction to direction file
        setdirgpio.close(); // close direction file
        return 0;
}

int GPIOClass::setval_gpio(string val)
{

    string setval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ofstream setvalgpio(setval_str.c_str()); // open value file for gpio
        if (setvalgpio < 0){
            cout << " OPERATION FAILED: Unable to set the value of GPIO"<< this->gpionum <<" ."<< endl;
            return -1;
        }

        setvalgpio << val ;//write value to value file
        setvalgpio.close();// close value file
        return 0;
}

int GPIOClass::getval_gpio(string& val){

    string getval_str = "/sys/class/gpio/gpio" + this->gpionum + "/value";
    ifstream getvalgpio(getval_str.c_str());// open value file for gpio
    if (getvalgpio < 0){
        cout << " OPERATION FAILED: Unable to get value of GPIO"<< this->gpionum <<" ."<< endl;
        return -1;
            }

    getvalgpio >> val ;  //read gpio value

    if(val != "0")
        val = "1";
    else
        val = "0";

    getvalgpio.close(); //close the value file
    return 0;
}

string GPIOClass::get_gpionum(){

return this->gpionum;

}

void sig_handler(int sig);

bool ctrl_c_pressed = false;

bool KnobTurned(int& counter, GPIOClass * bit1, GPIOClass * bit2, int& prevState)
{
  std::string b1, b2;
  bit1->getval_gpio(b1);
  bit2->getval_gpio(b2);
  //std::cout<<b1<<"  "<<b2<<std::endl;
  int num = (b1 == "1") ? (1<<1) : (0<<1);
  num |= (b2 == "1") ? (1) : (0);
  if(num == prevState){
    return false;
  }
  switch(num){
    case 0b00:
    if(prevState == 0b01){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b01:
    if(prevState == 0b11){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b11:
    if(prevState == 0b10){
      --counter;
    }
    else{
      ++counter;
    }
    break;
    case 0b10:
    if(prevState == 0b00){
      --counter;
    }
    else{
      ++counter;
    }
    break;
  }
  prevState = num;
  return true;
}

int main (void)
{
    std::string PINS[] = {"27","17","18","23","24","25","12","16","20","21"};
    GPIOClass * bit1 = new GPIOClass("5");
    GPIOClass * bit2 = new GPIOClass("6");
    bit1->export_gpio();
    bit1->setdir_gpio("in");
    bit2->export_gpio();
    bit2->setdir_gpio("in");
    GPIOClass* LEDs[10];
    int counter = 0;
    for(int i = 0; i < 10; ++i){
      LEDs[i] = new GPIOClass(PINS[i]);
      LEDs[i]->export_gpio();
      LEDs[i]->setdir_gpio("out");
      if(i != counter)
        LEDs[i]->setval_gpio("0");
      else{
        LEDs[i]->setval_gpio("1");
      }
    }
    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }

    string inputstate;
    //GPIOClass* gpio4 = new GPIOClass("4");
    //GPIOClass* gpio17 = new GPIOClass("17");

    //gpio4->export_gpio();
    //gpio17->export_gpio();

    //cout << " GPIO pins exported" << endl;
//
    //gpio17->setdir_gpio("in");
    //gpio4->setdir_gpio("out");
//
    //cout << " Set GPIO pin directions" << endl;
    bool on = false;
    int state = 0;
    while(1)
    {
        //usleep(500000);
        int old = counter;
        if(KnobTurned(counter, bit1, bit2, state))
        {
          LEDs[old]->setval_gpio("0");
          if(counter == 10){
            counter = 0;
          }
          if(counter <= -1){
            counter = 9;
          }
          //std::cout<<"turned "<<counter<<std::endl;
          LEDs[counter]->setval_gpio("1");
        }
        //gpio17->getval_gpio(inputstate);
        //cout << "Current input pin state is " << inputstate  <<endl;
        //if(inputstate == "0")
        //{
        //   cout << "input pin state is "Pressed ".n Will check input pin state again in 20ms "<<endl;
        //   usleep(20000);
        //           cout << "Checking again ....." << endl;
        //           gpio17->getval_gpio(inputstate);
        //    if(inputstate == "0")
        //    {
        //        cout << "input pin state is definitely "Pressed". Turning LED ON" <<endl;
        //        gpio4->setval_gpio("1");
//
        //        cout << " Waiting until pin is unpressed....." << endl;
        //        while (inputstate == "0"){
        //            gpio17->getval_gpio(inputstate);
        //        };
        //        cout << "pin is unpressed" << endl;
//
        //    }
        //    else
        //        cout << "input pin state is definitely "UnPressed". That was just noise." <<endl;
//
        //}
        //gpio4->setval_gpio("0");


         if(ctrl_c_pressed)
                    {
                        cout << "Ctrl^C Pressed" << endl;
                        cout << "unexporting pins" << endl;
                        //gpio4->unexport_gpio();
                        //gpio17->unexport_gpio();
                        //cout << "deallocating GPIO Objects" << endl;
                        //delete gpio4;
                        //gpio4 = 0;
                        //delete gpio17;
                        //gpio17 =0;
                        for(int i = 0; i < 10; ++i){
                          LEDs[i]->setval_gpio("0");
                          LEDs[i]->unexport_gpio();
                          delete LEDs[i];
                          LEDs[i] = 0;
                        }
                        bit1->unexport_gpio();
                        delete bit1;
                        bit1 = 0;
                        bit2->unexport_gpio();
                        delete bit2;
                        bit2 = 0;
                        break;

                    }

    }
    cout << "Exiting....." << endl;
    return 0;
}

void sig_handler(int sig)
{
    write(0,"nCtrl^C pressed in sig handlern",32);
    ctrl_c_pressed = true;
}