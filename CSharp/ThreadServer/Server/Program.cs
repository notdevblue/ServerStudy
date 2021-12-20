using System;
using System.Threading;

namespace Server
{

    public class Lock
    {
        AutoResetEvent _avalible = new AutoResetEvent(true);

        public void Acquire()
        {
            _avalible.WaitOne();
            // 문이 자동으로 닫힘

        }

        public void Release()
        {
            _avalible.Set(); // 다시 들어올 수 있게 켜줌
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            AutoResetEvent e = new AutoResetEvent(true);

            e.Set();
            e.WaitOne();
            e.Reset();

        }
    }
}
