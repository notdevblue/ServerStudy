using System;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    public class Lock
    {
        AutoResetEvent _avalible = new AutoResetEvent(true);

        public void Acquire()
        {
            _avalible.WaitOne(); // 한명이 들어가면 문을 닫음

        }

        public void Release()
        {
            
        }
    }
}
