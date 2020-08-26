using System;
using System.Runtime.InteropServices;
using System.Text;

namespace CSCaller
{
    class Program
    {
        [DllImport("CryptLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void Encrypt([MarshalAs(UnmanagedType.BStr)] string str, StringBuilder result);

        [DllImport("CryptLib.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        public static extern void Decrypt([MarshalAs(UnmanagedType.BStr)] string str, StringBuilder result);
        static void Main(string[] args)
        {
            //!This needs to be able to work
            //with chinese, russian etc.
            //For example, '你好' or 'Привет'

            Console.WriteLine("Please enter string to enctypt" + Environment.NewLine);
            string toEncrypt = Console.ReadLine();
            StringBuilder sbEncrypted = new StringBuilder(1024);
            Encrypt(toEncrypt, sbEncrypted);

            Console.WriteLine($"Encrypted string (in Base64) is: {sbEncrypted}" + Environment.NewLine);

            StringBuilder sbDecrypted = new StringBuilder(1024);
            Decrypt(sbEncrypted.ToString(), sbDecrypted);
            Console.WriteLine($"Decrypted string is: {sbDecrypted}");
        }
    }
}
