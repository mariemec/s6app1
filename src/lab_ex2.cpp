#include <cstdio>
#include <thread>
#include <mutex>
#include <queue>
#include <cstdlib>      // rand
#include <unistd.h>    // usleep
#include <condition_variable>

namespace {
    std::queue<int> queue_;
    std::mutex      mutex_;
    std::condition_variable cond;
}

void add_to_queue(int v)
{
    // Fournit un accès synchronisé à queue_ pour l'ajout de valeurs.
    
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(v);
}

void prod()
{
    // Produit 100 nombres aléatoires de 1000 à 2000 et les ajoute
    // à une file d'attente (queue_) pour traitement.
    // À la fin, transmet "0", ce qui indique que le travail est terminé.

    for (int i = 0; i < 100; ++i)
    {
        int r = rand() % 1001 + 1000;
        add_to_queue(r);
        cond.notify_one();
        // Bloque le fil pour 50 ms:
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    add_to_queue(0);
    cond.notify_one();
}

void cons()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(mutex_);
        cond.wait(ul, []{ return !queue_.empty(); });
        // On doit toujours vérifier si un objet std::queue n'est pas vide
        // avant de retirer un élément.
        int v = queue_.front(); // Copie le premier élément de la queue.
        queue_.pop();           // Retire le premier élément.

        printf("Reçu: %d\n", v);
        if (v == 0){break;}
    }

}

int main(int argc, char** argv)
{
    std::thread t_prod(prod);
    std::thread t_cons(cons);

    t_prod.join();
    t_cons.join();

    return 0;
}

