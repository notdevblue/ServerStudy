using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace TestProject
{
    class _ThreadPool
    {
        class A
        { 
            [ThreadStatic] private string str = "wa sans"; // ?

            public void ThreadProc(object stateInfo)
            {
                Console.WriteLine(str);
                str = "레식샌 왓다감";
                Thread.Sleep(250);
            }

            public void Call()
            {
                ThreadPool.SetMaxThreads(1, 1);
                ThreadPool.QueueUserWorkItem(ThreadProc);
                Thread.Sleep(500);
                ThreadPool.QueueUserWorkItem(ThreadProc);
            }
        }



        static void Main(string[] args)
        {
            A a = new A();

            a.Call();

            Console.WriteLine("Main Thread does some work, then sleeps.");
            Thread.Sleep(1000);

            Console.WriteLine("Main thread exits.");
        }

        
    }
}
