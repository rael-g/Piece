using System;
using System.Runtime.InteropServices;

public partial class Program
{
    private const string NativeLib = "wal_glfw";

    [LibraryImport(NativeLib, EntryPoint = "dummy_entry_point")]
    public static partial void DummyCall();

    public static void Main(string[] args)
    {
        try
        {
            // Attempt to load the DLL. The call will throw if the DLL is missing.
            DummyCall(); 

            // If we reach here, the DLL was loaded successfully.
            // For the "missing DLL" test, this means the test failed.
            Console.WriteLine("Native library 'wal_glfw' loaded successfully. This indicates an unexpected success.");
            Environment.Exit(0); // If the test is for 'DLL present', this would be success. For 'DLL missing', this is failure.
        }
        catch (Exception ex)
        {
            // If an exception occurs, the DLL was NOT loaded.
            // For the "missing DLL" test, this means the test *passed* (the program failed to load as expected).
            // For the "present DLL" test, this means the test *failed*.
            Console.WriteLine($"Caught exception while loading native library: {ex.Message}");
            Environment.Exit(1); // Indicate that the program failed to load the DLL.
        }
    }
}
