#ifndef CPINTI_SIGNALS_H
#define CPINTI_SIGNALS_H

namespace cpinti::signals
{
    void init();

    void panic(int signalnum, int ligne, char *fichier, char *fonction);
} // namespace cpinti::signals

#endif // CPINTI_SIGNALS_H
