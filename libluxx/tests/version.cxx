// Copyright © 2026 Markus Pawellek
//
// This file is part of `luxx`.
//
// `luxx` is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// `luxx` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `luxx`. If not, see <https://www.gnu.org/licenses/>.
//
import std;
import luxx;

int main() {
  using namespace std;

  // All the tests will be run asynchronously.
  // This leads to interleaved output in the terminal.
  // To prevent this for the version output, everything is written
  // into a 'stringstream' that is printed at the end at once.
  //
  stringstream output;
  output << "\nluxx version:\n\n";

#define CAPTURE(X) \
  output << left << setw(30) << #X << " = " << right << setw(40) << X << '\n'

  CAPTURE(luxx::version::number);
  CAPTURE(luxx::version::string);
  CAPTURE(luxx::version::id);
  CAPTURE(luxx::version::full);
  CAPTURE(luxx::version::major);
  CAPTURE(luxx::version::minor);
  CAPTURE(luxx::version::patch);
  CAPTURE(luxx::version::snapshot_sn);
  CAPTURE(luxx::version::snapshot_id);

  cout << output.str() << endl;
}
