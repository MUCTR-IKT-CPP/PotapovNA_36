#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// ������� ����� "��������"
class Animal {
public:
    string name;
    int age; 
    int hunger;
    int fatigue;
    bool busy; // ���� ��������� ���������

    Animal(string n, int a) :
        name(n), age(a), hunger(100), fatigue(0), busy(false) {}

   /*
    * ���� ���������
    *
    * @param double intensity ������������� ��������� ��������
    */
    virtual void play(double intensity) {
        if (busy) return; 
        fatigue = min(100, fatigue + static_cast<int>(50 * intensity)); 
        hunger = max (0, hunger - static_cast<int>(35 * intensity));  
    }

    /*
     * �������� ����
     */
    virtual void sleep() {
        if (busy) return; 
        fatigue = max(0, fatigue - 67); 
    }

    /*
     * �������� ���
     */
    virtual void eat() {
        if (busy) return; 
        hunger = min(100, hunger + 71); 
    }

    /*
     * �������� ������
     */
    void feed() {
        hunger = min(100, hunger + 6); 
        cout << name << " is fed by a visitor!" << endl;
    }

    /*
     * �������� �� ����� �� ���
     */
    bool needsSleep() const {
        return fatigue >= 80; 
    }

    /*
     * �������� �� ����� � ���
     */
    bool needsFood() const {
        return hunger <= 15; 
    }


    friend ostream& operator<<(ostream& os, const Animal& animal) {
        os << "<Animal> {Name: " << animal.name << ", Age: " << animal.age
            << ", Hunger: " << animal.hunger << ", Fatigue: " << animal.fatigue << "}\n";
        return os;
    }

    virtual ~Animal() {}
};

// �������� "���"
class Cat : public Animal {
public:
    Cat(string n, int a) : Animal(n, a) {}

    void play(double intensity) override {
        if (busy) return; 
        fatigue = min(100, fatigue + static_cast<int>(35 * intensity)); 
        hunger = max(0, hunger - static_cast<int>(80 * intensity)); 
    }

    void sleep() override {
        if (busy) return; 
        fatigue = max(0, fatigue - 82); 
        
    }

    void eat() override {
        if (busy) return; 
        hunger = min(100, hunger + 61); 
        
    }

    //friend ostream& operator<<(ostream& os, const Cat& cat) {
    //    os << "<Cat> " << static_cast<const Animal&>(cat);
    //    return os;
    //}
};

// �������� "������"
class Dog : public Animal {
public:
    Dog(string n, int a) : Animal(n, a) {}

    void play(double intensity) override {
        if (busy) return;
        fatigue = min(100, fatigue + static_cast<int>(40 * intensity)); 
        hunger = max(0, hunger - static_cast<int>(60 * intensity));  
    }

    void sleep() override {
        if (busy) return; 
            fatigue = max(0, fatigue - 71); 
        
    }

    void eat() override {
        if (busy) return; 
            hunger = min(100, hunger + 71); 
        
    }

  /*  friend ostream& operator<<(ostream& os, const Dog& dog) {
        os << "<Dog> " << static_cast<const Animal&>(dog);
        return os;
    }*/
};

// �������� "������"
class Wombat : public Animal {
public:
    Wombat(string n, int a) : Animal(n, a) {}

    void play(double intensity) override {
        if (busy) return; 
        fatigue = min(100, fatigue + static_cast<int>(40 * intensity)); 
        hunger = max(0, hunger - static_cast<int>(70 * intensity));  
    }

    void sleep() override {
        if (busy) return; 
            fatigue = max(0, fatigue - 62); 
        
    }

    void eat() override {
        if (busy) return;
            hunger = min(100, hunger + 44); 
        
    }

    //friend ostream& operator<<(ostream& os, const Wombat& wombat) {
    //    os << "<Wombat> " << static_cast<const Animal&>(wombat);
    //    return os;
    //}
};

// �������� "�����"
class Rat : public Animal {
public:
    Rat(string n, int a) : Animal(n, a) {}

    void play(double intensity) override {
        if (busy) return; 
        fatigue = min(100, fatigue + static_cast<int>(33 * intensity)); 
        hunger = max(0, hunger - static_cast<int>(87 * intensity));  
    }

    void sleep() override {
        if (busy) return; 
            fatigue = max(0, fatigue - 81); 
        
    }

    void eat() override {
        if (busy) return;
            hunger = min(100, hunger + 43); 
        
    }

    //friend ostream& operator<<(ostream& os, const Rat& rat) {
    //    os << "<Rat> " << static_cast<const Animal&>(rat);
    //    return os;
    //}
};

// ����� "����������"
class Visitor {
public:
    /*
    * ������ � ��������
    *
    * @param Animal animal ������ ��������
    * @param double intensity ������������� ��������� ��������
    */
    void playWithAnimal(Animal& animal, double intensity) {
        animal.busy = true; 
        animal.play(intensity);   
        animal.busy = false;
    }

    void feedAnimal(Animal& animal) {
        animal.busy = true; 
        animal.feed();      
        animal.busy = false; 
    }
};

// ����� "������"
class Enclosure {
public:
    vector<Animal*> animals;

    /*
     * ���������� ��������� � ������
     * 
     * @param Animal animal ������ ��������
     */
    void addAnimal(Animal* animal) {
        animals.push_back(animal);
    }

    /*
     * �������������� �������� ����� �����
     */
    void interact() {
        if (animals.size() > 1) {
            for (int i = 0; i < animals.size(); i++) {
                for (int j = i + 1; j < animals.size(); j++) {
                    if (rand() % 2 == 1) { // 50% ���� �� ��������������
                        animals[i]->busy = true;
                        animals[j]->busy = true;
                        // ����������� ��������� ��� ����� ��������
                        animals[i]->fatigue += 2; 
                        animals[j]->fatigue += 3;
                        /* cout << animals[i]->name << " interacts with another animal!" << endl;*/
                    }
                }
            }
        }
    }

    /*
     * �������� ��������
     */
    void inspect() {
        for (Animal* animal : animals) {
            if (animal->needsSleep()) {
                animal->sleep();  
                cout << animal->name << " is sleeping..." << endl;
            }
            if (animal->needsFood()) {
                animal->eat();    
                cout << animal->name << " is eating..." << endl;
            }
            //cout << *animal << endl; // ����� ���������� � ��������
        }
    }
};

// ����� "���������� �������"
class ContactZoo {
public:
    vector<Enclosure> enclosures; 
    double intensity; // ������������� ���������

    /*
     * ���������� �������
     */
    void addEnclosure(const Enclosure& enclosure) {
        enclosures.push_back(enclosure);
    }

    /*
     * ����� ������� ��������
     */
    void displayAnimalsStatus() {
        cout << "Current status of animals: \n*********************************************************************" << endl;
        for (const auto& enclosure : enclosures) {
            for (const Animal* animal : enclosure.animals) {
                cout << *animal; 
            }
        }
        cout << "*********************************************************************\n" << endl;
    }


    /*
     * ���������� ������ ��������
     */
    void simulateMonth() {
        Visitor visitor; 
        for (int day = 1; day <= 5; ++day) {
            cout << "Day " << day << ":\n";
            for (int hour = 0; hour < 8; hour++) {
                cout << "\t\t\t|Hour " << hour + 1 << "|\n\n";

                // ����� ������� ������������� ���� ��������
                displayAnimalsStatus();

                intensity = static_cast<float>(rand()) / RAND_MAX;

                for (auto& enclosure : enclosures) {
                    // ������ ��������
                    enclosure.inspect();
                    
                    // �������� ������ � ������ �������������
                    for (Animal* animal : enclosure.animals) {
                        // �������������� ��������
                        enclosure.interact();
                        animal->play(intensity);
                    }
                    // ���� � ��������� � ���������
                    for (Animal* animal : enclosure.animals) {
                        visitor.playWithAnimal(*animal, intensity); 
                        if (rand() % 2 == 1) { 
                            visitor.feedAnimal(*animal);
                        }
                    }
                }
                cout << "\n\n---------------------------\n\n";
            }
        }
    }

    /*
     * �������� ���������� ��������
     */
    void basicZoo() {
        // �������� �������� � ���������� ��������
        Enclosure catEnclosure;
        catEnclosure.addAnimal(new Cat("Cat_1", 3));
        catEnclosure.addAnimal(new Cat("Cat_2", 2));
        catEnclosure.addAnimal(new Cat("Cat_3", 4));
        addEnclosure(catEnclosure);

        Enclosure dogEnclosure;
        dogEnclosure.addAnimal(new Dog("Dog_1", 4));
        dogEnclosure.addAnimal(new Dog("Dog_2", 1));
        addEnclosure(dogEnclosure);

        Enclosure wombatEnclosure;
        wombatEnclosure.addAnimal(new Wombat("Wombat_1", 4));
        wombatEnclosure.addAnimal(new Wombat("Wombat_2", 1));
        wombatEnclosure.addAnimal(new Wombat("Wombat_3", 2));
        addEnclosure(wombatEnclosure);

        Enclosure ratEnclosure;
        ratEnclosure.addAnimal(new Rat("Rat_1", 1));
        addEnclosure(ratEnclosure);

    }

    /*
     * �������� ������
     */
    ~ContactZoo() {
        for (auto& enclosure : enclosures) {
            for (Animal* animal : enclosure.animals) {
                delete animal;
            }
        }
    }
};

int main() {
    ContactZoo zoo;

    srand(time(0));

    //��������� �������
    zoo.basicZoo();

    // ���������� ��������� � ������� ������ ������
    zoo.simulateMonth();

    return 0;
}
