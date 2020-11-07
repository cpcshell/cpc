#ifndef CPCDOS_SIGNALS
#define CPCDOS_SIGNALS

namespace cpinti::signals
{
    void init();

    void panic(int signalnum, int ligne, char *fichier, char *fonction);
} // namespace cpinti::signals

#endif /* CPCDOS_SIGNALS */
