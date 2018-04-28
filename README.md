# Managed-code-injection
 Inject a .NET assembly into a native process using the CLR Hosting API

## Step 1: Create the Managed Assembly (CodeInject)
 CodeInject: Managed code entry point (typical .NET class library) that provides code that is executed in the target process.

## Step 2: Create the Bootstrap DLL (Bootstrap)
 Bootstrap: Native DLL that executes code which invokes the .NET runtime that causes managed assembly to be loaded inside the target process.

- **CLRCreateInstance** Provides one of three interfaces: *ICLRMetaHost*, *ICLRMetaHostPolicy*, or *ICLRDebugging*.
- **ICLRMetaHost::GetRuntime** Gets the *ICLRRuntimeInfo* interface that corresponds to a particular version of CLR.
- **ICLRRuntimeInfo::IsLoadable** Indicates whether the runtime associated with this interface can be loaded into the current process.
- **ICLRRuntimeInfo::GetInterface** Loads the CLR into current process and returns runtime interface pointer to *ICLRRuntimeHost*.
- **ICLRRuntimeHost::Start** Launch the .NET runtime.
- **ICLRRuntimeHost::ExecuteInDefaultAppDomain** Load managed assembly and invoke the specified method.

## Step 3: Inject Bootstrap into target process

## Support
 Currently supports only 32-bit processes.