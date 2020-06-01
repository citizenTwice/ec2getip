/*

 EC2 public-IP fetcher
 
 MIT License

Copyright (c) 2020 Luigi Galli

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
 
*/
 
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>

#include <string>
#include <vector>
#include "citizentwice_str.h"
#include "credentials.h"

using namespace std;
using namespace citizentwice_str;

int main(int ac, char **av) {
  auto args = vector<string>{av + 1, av + ac};
  Aws::String instance_id{};
  bool verbose = false;
  string credfile{};
  for (auto a : args) {
    string val{};
    if (!(val = re_get_match("--credfile=(.*)", a)).empty()) {
      credfile = val;
    } else if (a == "-v") {
      verbose = true;
    } else {
      instance_id = a;
    }
  }
  if (
    instance_id.empty() ||
    credfile.empty()
  ) {
      cout
        << "Lookup public IP adr of a given EC2 instance\n"
        << "\nUsage:\n\n" 
        << "  ec2geip instance-id --credfile=/path/to/awskey.cfg [-v]\n"
        << "  instance-id  The instance-id for which the IP-lookup will be "
           "attempted\n"
        << "  -v           Verbose mode\n"
        << "  --credfile   Config file in the format:\n"
        << "                   [CREDENTIALS]\n"
        << "                   ACCESS_KEY_ID=Abc...\n"
        << "                   SECRET_ACCESS_KEY=Xyz...\n"
        << "\n";
    exit(1);
  }
  auto crd = creds_t::from_file(credfile.c_str());
  if (crd.err) {
    cerr << "Error reading credentials from " << credfile << "\n";
    cerr << crd.err_msg << "\n";
    exit(1);
  }
  if (verbose) {
    cout << "Using credentials: \n";
    cout << crd.to_string() << "\n";
  }

  Aws::SDKOptions options;
  Aws::InitAPI(options);
  {
    if (verbose) {
      cout << "Looking up instance: " << instance_id << "\n";
    }
    Aws::EC2::EC2Client ec2_client(Aws::Auth::AWSCredentials(crd.access_key_id, crd.secret_access_key)); 
    Aws::EC2::Model::DescribeInstancesRequest req;
    Aws::Vector<Aws::String> ids = {instance_id};
    req.SetInstanceIds(ids);
    auto outcome = ec2_client.DescribeInstances(req);
    if (outcome.IsSuccess()) {
      if (verbose) {
        cout << "Success.\n";
      }
      auto ip = outcome.GetResult()
                    .GetReservations()[0]
                    .GetInstances()[0]
                    .GetPublicIpAddress();
      if (verbose) {
        cout << "Instance " << instance_id << " -> public IP = " << ip << "\n";
      } else {
        cout << ip;
      }
    } else {
      cout << "Request error: " << outcome.GetError().GetExceptionName()
                << "\n"
                << outcome.GetError().GetMessage() << "\n";
    }
  }
  Aws::ShutdownAPI(options);
}
