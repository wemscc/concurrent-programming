
import java.util.Random;
class Buffer {
    static final int N = 10; // buffer
    float[] arr;
    private int count = 0, in = 0, out = 0;

    public Buffer() { // construtor
        this.arr = new float[N];
        for (int i = 0; i < N; i++) {
            arr[i] = 0.0F;
        }
    }

    public synchronized void Insere(int id, float item) {
        try {
            while (count == N) { // enquanto buffer estiver cheio espera
                System.out.println("Produtor[" + id + "] está bloqueado");
                wait();
            }
            System.out.println("Produtor[" + id + "] se desbloqueou");
            arr[in] = item;
            in = (in + 1) % N;
            count++;
            notifyAll();
        } catch (InterruptedException e) {
        }
    }

    public synchronized float Retira(int id) {
        float item;
        try {
            while (count == 0) {
                System.out.println("Consumidor[" + id + "] está bloqueado");
                wait();

            }
            System.out.println("Consumidor[" + id + "] se desbloqueou");

            item = arr[out];
            arr[out] = 0;
            out = (out + 1) % N;
            count--;
            notifyAll();
            return item;
        } catch (InterruptedException e) {
            return -1;
        }
    }

    public synchronized void ImprimeBuffer() { // imprime o buffer
        for (int i = 0; i < N; i++) {
            System.out.print(arr[i] + " ");
        }
    }
}

    class Produtor extends Thread {

        int id;
        int qntItems;
        int delayTime;
        Buffer buffer;

        public Produtor(int id, int delayTime, Buffer buffer, int qntItems) {
            this.id = id;
            this.qntItems = qntItems;
            this.delayTime = delayTime;
            this.buffer = buffer;
        }

        public void run() {
            System.out.println("\nProdutor " + id +" quer inserir\n");
            Random random = new Random();

            try {
                for (int i=0;i<qntItems;i++) {
                    float r = random.nextFloat();
                    this.buffer.Insere(this.id, r);
                    sleep(this.delayTime);
                    System.out.println("\nProdutor " + id + " inseriu " + r);
                }
            } catch (InterruptedException e) {
                System.out.println("T[" + this.id + "] produtora sofreu exceção na execução");
            }
        }
    }

    class Consumidor extends Thread {
        int id;
        int qntItems;
        int delayTime;
        Buffer buffer;

        public Consumidor(int id, int delayTime,int qntItems, Buffer buffer) {
            this.id = id;
            this.delayTime = delayTime;
            this.qntItems = qntItems;
            this.buffer = buffer;
        }

        public void run() {
            System.out.println("\nConsumidor " + id +" quer consumir\n");
            float aux = 0;
            try {
                for (int i = 0 ; i<qntItems;i++){
                    aux = this.buffer.Retira(this.id);
                    sleep(this.delayTime);
                    System.out.println("\nConsumidor " + id +" consumiu " + aux + "\n");
                }
            } catch (InterruptedException e) {
                System.out.println("T[" + this.id + "] consumidora sofreu exceção na execução");
            }
        }
    }
    class ThreadSafe {
    static final int PROD = 3; // quantidade de produtores
    static final int CONS = 1; // quantidade de consumidores

    public static void main(String[] args) {
        Buffer buffer = new Buffer();
        buffer.ImprimeBuffer();
        Produtor[] p = new Produtor[PROD];
        Consumidor[] c = new Consumidor[CONS];

        for (int i = 0; i < PROD; i++) {
            p[i] = new Produtor(i, 1 ,buffer, 5);
            p[i].start();
        }

        for (int i = 0; i < CONS; i++) {
            c[i] = new Consumidor(i, 1,5 , buffer);
            c[i].start();
        }

        try { // espera as threads terminarem
            for (int i = 0; i < PROD; i++) {
                p[i].join();
            }
            for (int i = 0; i < CONS; i++) {
                c[i].join();
            }
        } catch (InterruptedException e) {
            System.out.println("Função main teve exceção");
        }
        buffer.ImprimeBuffer();
    }

}




