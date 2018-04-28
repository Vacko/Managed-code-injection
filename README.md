# Managed-code-injection
 Inject a .NET assembly into a native process using the CLR Hosting API

## Step 1: Create the Managed Assembly (CodeInject)
- CodeInject: Managed code entry point (typical .NET class library) that provides code that is executed in the target process.

## Step 2: Create the Bootstrap DLL (Bootstrap)
- Bootstrap: Native DLL that executes code which invokes the .NET runtime, and the .NET runtime causes managed assembly to be loaded.

## Support
 Currently supports only 32-bit processes.