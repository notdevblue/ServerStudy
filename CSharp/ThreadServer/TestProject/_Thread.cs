using System;
using System.Diagnostics;
using System.Threading;
using System.Threading.Tasks;


namespace TestProject
{
    class _Thread
    {
        public static void ThreadProc()
        {
            for (int i = 0; i < 10; ++i)
            {
                Console.WriteLine("ThreadProc: {0}", i);
                System.Threading.Thread.Sleep(0);
            }
        }

        public static void PrintHelloTenTimes(object obj)
        {
            for (int i = 0; i < 10; ++i)
            {
                Console.WriteLine(obj as string, i + 1);

            }
        }

        public static void ExecuteInForeground(object obj)
        {
            int interval;
            try
            {
                interval = (int)obj;
            }
            catch (InvalidCastException)
            {
                interval = 5000;
            }

            var sw = Stopwatch.StartNew();

            do
            {
                Console.WriteLine("Thread {0}: {1}, Priority {2}",
                    System.Threading.Thread.CurrentThread.ManagedThreadId,
                    System.Threading.Thread.CurrentThread.ThreadState,
                    System.Threading.Thread.CurrentThread.Priority);
                System.Threading.Thread.Sleep(500);
            }
            while (sw.ElapsedMilliseconds <= interval);
            sw.Stop();
        }

        public static void BackgroundThread()
        {
            while (true)
            {
                Console.WriteLine("I am Background Thread {0}, and {1}. Priority {2}",
                    System.Threading.Thread.CurrentThread.ManagedThreadId,
                    System.Threading.Thread.CurrentThread.ThreadState,
                    System.Threading.Thread.CurrentThread.Priority);
                System.Threading.Thread.Sleep(500);
            }
        }

        public static void ForegroundThread()
        {
            for (int i = 0; i < 4; ++i)
            {
                Console.WriteLine("I am Foreground Thread {0}, and {1}. Priority {2}",
                    System.Threading.Thread.CurrentThread.ManagedThreadId,
                    System.Threading.Thread.CurrentThread.ThreadState,
                    System.Threading.Thread.CurrentThread.Priority);
                System.Threading.Thread.Sleep(500);
            }
        }


        static void _Main(string[] args)
        {
            //Thread t1 = new Thread(new ThreadStart(ThreadProc));
            //Thread t2 = new Thread(PrintHelloTenTimes);
            //Thread t3 = new Thread(ExecuteInForeground);
            System.Threading.Thread t4 = new System.Threading.Thread(BackgroundThread);
            System.Threading.Thread t5 = new System.Threading.Thread(ForegroundThread);
            t4.IsBackground = false;
            

            //t1.Start();
            //t2.Start("Hello! {0}");
            //t3.Start(1500);
            t4.Start();
            t5.Start();

            System.Threading.Thread.Sleep(1000);
            t4.IsBackground = true;


            //Thread.Sleep(1000);
            //Console.WriteLine("Main thread ({0}) exiting...", Thread.CurrentThread.ManagedThreadId);



            //for(int i = 0; i < 4; ++i)
            //{
            //    Console.WriteLine("Main thread: Do some work");
            //    Thread.Sleep(0);
            //}

            //Console.WriteLine("Main thread: Call Join(), to wait until ThreadProc ends.");
            //t1.Join();
            //t2.Join();
            //Console.WriteLine("Main thread: ThreadProc.Join has returned. Press Enter to end program.");
            //Console.ReadLine();
        }
    }
}
