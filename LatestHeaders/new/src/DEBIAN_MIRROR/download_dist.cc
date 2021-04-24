#include "./all.hh"

int main() {
    MainWorker slave;
    slave.download_dists();
    //slave.download_list();
    return 0;
}
