Method 1: (async () => { ... })().catch(console.error);

Method 2: Promise.resolve().then(async () => { ... }).catch(console.error);

Method 3: rashadDo(async () => { ... }).catch(console.error);
