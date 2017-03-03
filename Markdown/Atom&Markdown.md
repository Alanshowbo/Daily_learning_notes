# Atom and Markdown Learning Notes

Created in 2017/2/15 by YunShuiXin
## Markdown command
**********************************

+ cmd line in atom on windows shortcut: ctrl+shift+p
+ Markdown preview in atom: Markdown preview: toggle
+ in Markdown a "#" means HEADING1，"##" means HEADING2 ...
+ list："*" or "+" or "-" means lists without order
+ ">" means quote
> this is quote
+ a web address can be directly recognized

[GITHUB YunShuiXin](https://github.com/ShuiXinYun 'GITHUB Website')

+ *information* use one "*" means Italic
+ **information** use two "**" means Bold

+ use `` to embody code make it present in code style

`code code`
+ triple "*" or "-" is split line


-----------------------

## Atom install
### On Linux(Ubuntu)
#### Build
+ `sudo apt-get install build-essential git libgnome-keyring-dev fakeroot rpm libx11-dev libxkbfile-dev`
+ `git clone https://github.com/atom/atom.git`
+ `cd atom`
+ `script/build`

#### Deb
+ [Atom中文社区源](https://npm.taobao.org/mirrors/atom)
+ sudo dpkg --install atom-amd64.deb

### On windows
[Atom中文社区源](https://npm.taobao.org/mirrors/atom)

---

## Atom Package install
+ settings install(blocked)
+ Install GIT and NPM(NodeJS)
        cd ***/.atom/packages
        git clone <package link>
        cd <package directory>
        npm install

## Markdown LaTeX Render
        cd <package directory>
        npm install fs-plus
        git clone <markdown preview plus link>
