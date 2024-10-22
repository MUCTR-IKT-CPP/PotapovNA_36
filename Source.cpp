#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Target {
public:
    virtual void receiveDamage(int damage) = 0;
    virtual double getHitChance() const = 0;
    virtual bool isAlive() const = 0;
    virtual ~Target() = default;
};

//����������� ������
class StaticTarget : public Target {
private:
    string name;
    int maxHits;    // ������������ ���������� ���������
    int currentHits; // ������� ���������� ���������
    double visibility; // ���������

public:
    StaticTarget(const string& name, int maxHits, double visibility)
        : name(name), maxHits(maxHits), currentHits(0), visibility(visibility) {}

    /*
     * ��������� �����
     *
     * @param int damage ���������� �����
     */
    void receiveDamage(int damage) override {
        currentHits++;
        cout << name << " received " << damage << " damage. " << endl;
    }

    /*
     * ���� ���������
     *
     * @return �������� ������
     */
    double getHitChance() const override {
        return visibility;  
    }

    /*
     * �������� �� ��������� ������
     *
     * @return ������� ������� ������
     */
    bool isAlive() const override {
        return currentHits < maxHits; 
    }
};

// ����������� ������
class MovingTarget : public Target {
private:
    string name;
    int maxHits;    // ������������ ���������� ���������
    int currentHits; // ������� ���������� ���������
    double visibility; // ���������� �������������� ��� ��������
    double speed;      // ���������� �������������� ��� �����������

public:
    MovingTarget(const string& name, int maxHits, double visibility, double speed)
        : name(name), maxHits(maxHits), currentHits(0), visibility(visibility), speed(speed) {}

    /*
     * ��������� �����
     *
     * @param int damage ���������� �����
     */
    void receiveDamage(int damage) override {
        currentHits++;
        cout << name << " received " << damage << " damage." << endl;
    }

    /*
     * ���� ���������
     *
     * @return �������� ������ � ������ �������� ��������
     */
    double getHitChance() const override {
        return visibility / (1 + speed); // ���� ��������� ����������� � ����������� ��������
    }

    /*
     * �������� �� ��������� ������
     *
     * @return ������� ������� ������
     */
    bool isAlive() const override {
        return currentHits < maxHits;
    }
};

// ��������� ������
class SmallTarget : public Target {
private:
    string name;
    int maxHits;
    int currentHits;
    double visibility; // ���������� �������������� ��� ��������� �������
    double size;      // ���������� �������������� ��������� ������

public:
    SmallTarget(const string& name, int maxHits, double visibility, double size)
        : name(name), maxHits(maxHits), currentHits(0), visibility(visibility), size(size) {}

    /*
     * ��������� �����
     *
     * @param int damage ���������� �����
     */
    void receiveDamage(int damage) override {
        currentHits++;
        cout << name << " received " << damage << " damage." << endl;
    }

    /*
     * ���� ���������
     *
     * @return �������� ������ � ������ ������� ������
     */
    double getHitChance() const override {
        return visibility - size; 
    }

    /*
     * �������� �� ��������� ������
     *
     * @return ������� ������� ������
     */
    bool isAlive() const override {
        return currentHits < maxHits;
    }
};

class Weapon {
private:
    int maxAmmo;
    int currentAmmo;

public:
    Weapon(int maxAmmo) : maxAmmo(maxAmmo), currentAmmo(0) {}

    /*
     * ����������� ������
     *
     * @param int ammo ���������� ��������
     */
    void reload(int ammo) {
        currentAmmo = min(maxAmmo, currentAmmo + ammo);
        cout << "Weapon reloaded. Current ammo: " << currentAmmo << "\n" << endl;
    }

    /*
     * ��������� ��������
     *
     * @param Target& target ������
     * @param int damage ���������� �����
     */
    void fireSingle(Target& target, int damage) {
        if (currentAmmo > 0 && target.isAlive()) {
            double chance = target.getHitChance();
            if (hit(chance)) {
                target.receiveDamage(damage);
                cout << "Fired single shot. Remaining ammo: " << --currentAmmo << "\n" << endl;
            }
            else {
                cout << "Missed the target!\n" << endl;
            }
        }
        else if (!target.isAlive()) {
            cout << "Target is destroyed!\n" << endl;
        }
        else {
            cout << "Out of ammo!" << endl;
            reload(maxAmmo);
        }
    }

    /*
     * �������� ���������
     *
     * @param Target& target ������
     * @param int damage ���� ����
     */
    void fireBurst(Target& target, int damagePerBullet) {
        if (currentAmmo >= 3 && target.isAlive()) {
            double chance = target.getHitChance();
            if (hit(chance)) {
                int totalDamage = 3 * damagePerBullet;
                target.receiveDamage(totalDamage);
                cout << "Fired burst. Total damage dealt: " << totalDamage << ". Remaining ammo: "
                    << (currentAmmo - 3) << "\n" << endl;
            }
            else {
                cout << "Burst fire missed. \n" << endl;
            }
            currentAmmo -= 3;
        }
        else if (!target.isAlive()) {
            cout << "Target is destroyed!\n" << endl;
        }
        else {
            cout << "Not enough ammo for burst!\n" << endl;
            reload(maxAmmo);
        }
    }

    /*
     * �������� ���������
     *
     * @param double chance
     * @return ��������� ���������
     */
    bool hit(double chance) {
        double randomHit = static_cast<double>(rand()) / RAND_MAX;
        cout << "Random hit value: " << randomHit << ", Hit chance: " << chance << endl;
        return randomHit < chance;
    }
};

class ShootingRange {
private:
    Weapon& weapon;
    Target* target;

public:
    ShootingRange(Weapon& weapon, Target* target)
        : weapon(weapon), target(target) {}

    /*
     * �������� � ������
     *
     * @param int damage ���������� �����
     */
    void shootAtTarget(int damage) {
        if (target->isAlive()) {
            weapon.fireSingle(*target, damage);
        }
    }

    /*
     * ����� ������
     *
     * @return ������
     */
    Target* getTarget() const {
        return target;
    }
};

class ShootingSimulator {
private:
    Weapon& weapon;
    vector<unique_ptr<Target>>& targets;

public:
    ShootingSimulator(Weapon& weapon, vector<unique_ptr<Target>>& targets)
        : weapon(weapon), targets(targets) {}

    /*
     * ��������� ��������
     *
     */
    void simulate() {
        int destroyedTargets = 0;
        int totalDamage = 0;

        // ��������� ���������� �������� �������� ���������� �������� � ������ ���������
        int singleShotSpeed = 1 + rand() % 4; 
        int burstShotSpeed = 1 + rand() % 2;
        cout << "Single shot speed: " << singleShotSpeed << ", Burst shot speed: " << burstShotSpeed << endl;

        for (int i = 1; i < 10; i++) {
            cout << "\n\n    -- Second " << i << " --" << endl;

            for (int i = 0; i < singleShotSpeed; i++) {
                for (auto& target : targets) {
                    ShootingRange range(weapon, target.get()); // ��������� ��������� �� ������
                    range.shootAtTarget(5); // ���� 5 �� ������ ������
                }
            }

            // ��������� ������ �������� � ������������ 35%
            if (static_cast<double>(rand()) / RAND_MAX < 0.35) {
                cout << "**Changing firing mode!**" << endl;
                for (int i = 0; i < burstShotSpeed; i++) {
                    for (auto& target : targets) {
                        ShootingRange range(weapon, target.get());
                        weapon.fireBurst(*target, 5); // ���� 5 �� ������ ������
                    }
                }
            }

            totalDamage += (5 * (singleShotSpeed + burstShotSpeed)); // ��� ���������� ��������� ����� �� ��������

            cout << endl;
        }

        // ������� ����������� ������� � ������ �����
        for (const auto& target : targets) {
            if (!target->isAlive()) {
                destroyedTargets++;
            }
        }

        // ����� �����������
        int remainingTargets = targets.size() - destroyedTargets;
        double averageDamage = totalDamage / static_cast<double>(targets.size());

        cout << "Destroyed targets: " << destroyedTargets << endl;
        cout << "Remaining targets: " << remainingTargets << endl;
        cout << "Average damage dealt: " << averageDamage << endl;
    }

    /*
     * ���������� �������
     * 
     * @param int count ���������� �������
     */
    void createTargets(int count) {
        for (int i = 0; i < count; i++) {
            targets.push_back(createRandomTarget());
        }
    }

    /*
     * �������� �������
     *
     */
    unique_ptr<Target> createRandomTarget() {
        // ��������� ��������� �����
        int type = rand() % 3; // ��� ������
        int maxHits = 3 + rand() % 3; // ���-�� ��������� ��� ����������
        double visibility = 0.5 + (static_cast<double>(rand()) / RAND_MAX) * 0.5; // ���������
        double size = 0.1 + (static_cast<double>(rand()) / RAND_MAX) * 0.4; // ������
        double speed = 0.1 + (static_cast<double>(rand()) / RAND_MAX) * 0.9; // ��������

        if (type == 0) {
            return make_unique<StaticTarget>("Static Target", maxHits, visibility);
        }
        else if (type == 1) {
            return make_unique<MovingTarget>("Moving Target", maxHits, visibility, speed);
        }
        else {
            return make_unique<SmallTarget>("Small Target", maxHits, visibility, size);
        }
    }
};

int main() {
    srand(time(0)); 

    Weapon weapon(15);
    weapon.reload(15);
    int targetCount = 7;

    vector<unique_ptr<Target>> targets;
    ShootingSimulator simulator(weapon, targets);

    simulator.createTargets(targetCount);
    simulator.simulate();

    return 0;
}
