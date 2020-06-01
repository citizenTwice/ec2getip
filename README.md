## EC2 Public IP fetcher

When I started playing with EC2, looking up instance metadata using the AWS command line utility didn't work very reliably for me, so I decided to write a self contained tool - for ease of scripting - which I still use today.

### Usage
<pre>
% ./ec2getip           
Lookup public IP adr of a given EC2 instance

Usage:

  ec2geip instance-id --credfile=/path/to/awskey.cfg [-v]
  instance-id  The instance-id for which the IP-lookup will be attempted
  -v           Verbose mode
  --credfile   Config file in the format:
                   [CREDENTIALS]
                   ACCESS_KEY_ID=Abc...
                   SECRET_ACCESS_KEY=Xyz...

</pre>

Example:

```bash
% ec2getip --credfile=$HOME/.aws/creds.cfg i-086458aafeeb3c2af 
54.157.212.180%                                                                                                                                                                               

% alias ssh_ubuntu='ssh ubuntu@$( ec2getip --credfile=$HOME/.aws/creds.cfg i-086458aafeeb3c2af  )  -i $HOME/.ssh/id_ec2.txt '

% ssh_ubuntu

The authenticity of host '54.157.212.180 (54.157.212.180)' can't be established.
ECDSA key fingerprint is SHA256:LMA8C1/EedSEqXhocyDPAZEmmyJ90tZlIh1FDjy3wow.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '54.157.212.180' (ECDSA) to the list of known hosts.

Welcome to Ubuntu 18.04.3 LTS (GNU/Linux 5.3.0-1019-aws x86_64)
ubuntu@ip-172-31-1-103:~$ 
```

### Building
I've successfully compiled this utility on Linux and macOS. I believe a Windows binary can be built with little to no Makefile adjustments, particularly if you are using CygWin.
Requirements:
* C++ compiler
* [AWS C++ SDK](https://www.google.com/search?q=AWS+C%2B%2B+SDK)
* Other libs: OpenSSL, libcurl, pthread

All of the above, with the possible exception of the AWS C++ SDK, should be readily installable via your package manager of choice.
I've compiled and installed the AWS SDK myself and added two variables to the Makefile `AWS_INC_DIR, AWS_LIB_DIR` so it can be easily modified to match your configuration. 
The SDK is a quite a large project and compiling it may take a long time, taking up large amounts of memory and CPU time. This can be mitigated by building only the needed parts of the SDK. Also, I recommend building the static-linking version of the libraries, to avoid making the final product dependent on the AWS DLL's at runtime, so in my case I've used `cmake -DSTATIC_LINKING=1  -DBUILD_ONLY="s3;ec2" ../awsrepo/aws-sdk-cpp`

### Feedback/Contact
Thanks for visting.
* Email: [LG@citizentwice.nl](mailto:LG@citizentwice.nl)

### License
MIT

