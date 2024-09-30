#ifndef MNTENTRY_H
#define MNTENTRY_H

#include <string>

class MNTEntry {
public:
    MNTEntry();
    MNTEntry(const std::string& name, int pp, int kp, int mdtp, int kpdtp);

    int getPp() const;
    int getKp() const;
    int getMdtp() const;
    int getKpdtp() const;

private:
    std::string name;
    int pp, kp, mdtp, kpdtp;
};

#endif // MNTENTRY_H
