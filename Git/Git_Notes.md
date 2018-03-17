# Git Notes
> Created 2018/03/17
## Git Install
`sudo apt-get install git`

## Git Config
```
git config --global user.name "Your Name"
git config --global user.email "email@example.com"
```

## Create repository
通过`git init`命令把这个目录变成Git可以管理的仓库

## Add & Commit
```
git add filename 
# git add *
git commit -m "description"
```

## git status, git diff
git status命令可以让我们时刻掌握仓库当前的状态,git diff可以查看difference，显示的格式正是Unix通用的diff格式

## git log, git reset, git reflog
+ git log命令显示从最近到最远的提交日志
+ Git必须知道当前版本是哪个版本，在Git中，用HEAD表示当前版本，也就是最新的提交3628164...882e1e0（注意我的提交ID和你的肯定不一样），上一个版本就是HEAD^，上上一个版本就是HEAD^^，当然往上100个版本写100个^比较容易数不过来，所以写成HEAD~100。
`git reset --hard HEAD^`
+ 现在，回退到了某个版本，想恢复到新版本怎么办？
  + 只要命令行窗口还没关掉，可以顺着往上找,找到的commit id是3628164...,版本号没必要写全，前几位就可以了，Git会自动去找.
    ```
    git reset --hard 3628164
    ```
  + Git提供了一个命令git reflog用来记录你的每一次命令,找到的commit id.

## 工作区,版本库,暂存区
每次修改，如果不add到暂存区，那就不会加入到commit中

## 撤销修改
+ `git checkout`: 让文件回到最近一次git commit或git add时的状态.`git checkout -- README.md`
+ `git reset HEAD file`: git add到暂存区.`git reset HEAD README.md`
+ `git reset ...`: git commit
    ```
    场景1：当你改乱了工作区某个文件的内容，想直接丢弃工作区的修改时，用命令git checkout -- file。
    场景2：当你不但改乱了工作区某个文件的内容，还添加到了暂存区时，想丢弃修改，分两步，第一步用命令git reset HEAD file，就回到了场景1，第二步按场景1操作。
    场景3：已经提交了不合适的修改到版本库时，想要撤销本次提交，参考版本回退，不过前提是没有推送到远程库。
    ```
## 删除文件
一般情况下，通常直接在文件管理器中把没用的文件删了，或者用rm命令删了:
```
rm test.txt
```
这个时候，Git知道你删除了文件，因此，工作区和版本库不一致，git status命令会立刻告诉你哪些文件被删除.
现在你有两个选择，一是确实要从版本库中删除该文件，那就用命令git rm删掉，并且git commit：
```
git rm README.md
git commit -m "README.md removed"
```
另一种情况是删错了，因为版本库里还有，所以可以轻松地把误删的文件恢复到最新版本：
```
git checkout -- README.md
```
命令git rm用于删除一个文件。如果一个文件已经被提交到版本库，那么不用担心误删，但是要小心，只能恢复文件到最新版本，**会丢失最近一次提交后你修改的内容**
## git & Github
由于本地Git仓库和GitHub仓库之间的传输是通过SSH加密的，所以需要一点设置:
1. 创建SSH Key:在用户主目录下，看看有没有.ssh目录，如果有，再看看这个目录下有没有id_rsa和id_rsa.pub这两个文件，如果已经有了，可直接跳到下一步。如果没有，打开Shell（Windows下打开Git Bash），创建SSH Key：
    ```
    ssh-keygen -t rsa -C "youremail@example.com"
    ```
    需要把邮件地址换成你自己的邮件地址，然后一路回车，使用默认值即可，由于这个Key也不是用于军事目的，所以也无需设置密码。如果一切顺利的话，可以在用户主目录里找到.ssh目录，里面有id_rsa和id_rsa.pub两个文件，这两个就是SSH Key的秘钥对，id_rsa是私钥，不能泄露出去，id_rsa.pub是公钥，可以放心地告诉任何人。
2. 登陆GitHub，打开“Account settings”，“SSH Keys”页面,点“Add SSH Key”，填上任意Title，在Key文本框里粘贴id_rsa.pub文件的内容.
> 为什么GitHub需要SSH Key呢？因为GitHub需要识别出你推送的提交确实是你推送的，而不是别人冒充的，而Git支持SSH协议，所以，GitHub只要知道了你的公钥，就可以确认只有你自己才能推送。当然，GitHub允许你添加多个Key。假定你有若干电脑，你一会儿在公司提交，一会儿在家里提交，只要把每台电脑的Key都添加到GitHub，就可以在每台电脑上往GitHub推送了。
### Github 添加远程库
现在的情景是，你已经在本地创建了一个Git仓库，又想在GitHub创建一个Git仓库，并且让这两个仓库进行远程同步，这样GitHub上的仓库既可以作为备份，又可以让其他人通过该仓库来协作，一举多得。

首先，登陆GitHub，然后，找到“Create a new repo”按钮，创建一个新的仓库：在Repository name填入learngit，其他保持默认设置，点击“Create repository”按钮，就成功地创建了一个新的Git仓库.

目前GitHub上的这个learngit仓库还是空的，可以从这个仓库克隆出新的仓库，也可以把一个已有的本地仓库与之关联，然后把本地仓库的内容推送到GitHub仓库。现在，我们根据GitHub的提示，在本地的learngit仓库下运行命令：
```
git remote add origin git@github.com:GithubAccountName/******.git
```
关联后，使用命令`git push -u origin master`第一次推送master分支的所有内容；此后，每次本地提交后，只要有必要，就可以使用命令`git push origin master`推送最新修改；
### 从远程库克隆
git clone git@github.com:GithubAccountName/******.git

还可以用https://github.com/GithubAccountName/******.git这样的地址。实际上，Git支持多种协议，默认的git://使用ssh，但也可以使用https等其他协议。使用https除了速度慢以外，还有个最大的麻烦是每次推送都必须输入口令，但是在某些只开放http端口的公司内部就无法使用ssh协议而只能用https。