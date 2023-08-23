<!---
  LevelAPI - Geometry Dash level cacher with search functionality and more.
  Copyright (C) 2023  Sergei Baigerov

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
-->

# Level API

**Level API** — simple to use **level and account cacher.** This project is being rewritten since December 2022 from JS to C++ with big improvements over old one in performance and code readability.
<br>

# Features
- Recording level data in `.gmd2` format
- Multiple servers support (or Nodes)
  - Official Basement GDPS (Подвал ГДшеров) support
  - Official GD server in 2.1 and 2.2 modes
  - 1.9 GDPS
  - Other server using default GD server implementatons
- Discord support
  - Notifications
  - Slash Commands
- SQlite usage – no more hard MySQL stuff
- Easy configuration – example config file is provided
- Asynchronous – each node is processed in different threads (network thread, SQL thread, etc.)
- **More features soon!**

<!---
# API
**Documentation for Level API available here:** [click!](https://sergeymc9730.github.io/levelapi/#/)
-->

# Demo server

Main server have **over `190k` levels** in total **size of `775` megabytes of storage.**
<br>
Main server is [placed here](https://lapi.vse-serii-naruto.ru/api/v1/stats)!
