#include "MNTEntry.h"

MNTEntry::MNTEntry() : name(""), pp(0), kp(0), mdtp(0), kpdtp(0) {}

MNTEntry::MNTEntry(const std::string& name, int pp, int kp, int mdtp, int kpdtp)
    : name(name), pp(pp), kp(kp), mdtp(mdtp), kpdtp(kpdtp) {}

int MNTEntry::getPp() const { return pp; }
int MNTEntry::getKp() const { return kp; }
int MNTEntry::getMdtp() const { return mdtp; }
int MNTEntry::getKpdtp() const { return kpdtp; }
