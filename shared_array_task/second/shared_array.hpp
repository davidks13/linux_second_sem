#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <string>
#include <semaphore.h>


class shared_array {
    public:
        shared_array(const char* arr_name, int size) : shared_size(size) {
            shared_fd = shm_open(arr_name, O_CREAT | O_RDWR, 0644);
            shared_fd_check();

            mmap_arr = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shared_fd, 0);
            mmap_check();

            shared_arr = (int*)mmap_arr;
            memset(shared_arr, 0, PAGE_SIZE);

            sp = sem_open(arr_name, O_CREAT | O_RDWR, 0666, 1);
            sem_check();
        }

        ~shared_array() {
            sem_close(sp);
        
            munmap(mmap_arr, PAGE_SIZE);

            close(shared_fd);
        }

        const int size() {
            return shared_size;
        }

        const int &operator[](const int &index) const {
            if(index >= shared_size) {
                throw std::out_of_range("The given index is out of range\n");
            }

            sem_wait(sp);
            int& element = shared_arr[index];
            sem_post(sp);
        
            return element;
        }

        int &operator[](const int &index) {
            if(index >= shared_size) {
                throw std::out_of_range("The given index is out of range\n");
            }

            sem_wait(sp);
            int& element = shared_arr[index];
            sem_post(sp);
        
            return element;
        }

    private:
        int PAGE_SIZE = 4096;
        int shared_fd;
        void* mmap_arr;
        int* shared_arr;
        int shared_size;
        sem_t* sp;


        void shared_fd_check() {
            if (shared_fd < 0) {
                std::cerr << strerror(errno) << std::endl;
                exit(0);
            }

            if (ftruncate(shared_fd, PAGE_SIZE) < 0) {
                std::cerr << strerror(errno) << std::endl;
                exit(0);
            }
        }

        void mmap_check() {
            if (mmap_arr == MAP_FAILED) {
                std::cerr << strerror(errno) << std::endl;
                exit(0);
            }
        }

        void sem_check() {
            if (sp == SEM_FAILED) {
                std::cerr << strerror(errno) << std::endl;
                close(shared_fd);
                munmap(mmap_arr, PAGE_SIZE);
                exit(0);
            }
        }
};