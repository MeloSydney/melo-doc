

//TODO - git general


git clone **-b e04_sdk1.5_master **"ssh://fan.shi@gerrit.ecarxgroup.com:29418/ICVSBG/CHUDC/AP/E04/kernel" && scp -p -P 29418 [fan.shi@gerrit.ecarxgroup.com](mailto:fan.shi@gerrit.ecarxgroup.com):hooks/commit-msg "kernel/.git/hooks/" // 拉取分支
git clone --recurse-submodules https://xxx/xxx


git branch -a  // 查看所有分支
git branch -r // 查看远端分支
git branch --set-upstream-to origin/远程分支名  本地分支名 // 追踪分支
git branch --set-upstream-to=origin/<branch> e05_xen_v2000_project_dev

git checkout -b 本地分支名 origin/远程分支名  // 本地创建分支 并追踪远程分支

git merge  合并分支名 // 分支merge

git push origin 远程仓库名

git fetch --all // 拉取远端所有信息 但是不合入

git reset --hard $commitID  // 回退到某个提交

/* 回退到某个时间点前 */

./repo forall -c "git clean -df && git checkout -f && git reset --hard"; ./repo sync -j 48
./repo forall -c 'commitID=`git log --before "2023-05-08 22:00" -1 --pretty=format:"%H"`; git reset --hard $commitID'


git diff drivers/input/ > getuser.patch

/* 显示mm下面的patch */
tig mm/


git format-patch -1 abcdefg12345 /* 根据hash生成patch */
git format-patch HEAD^   /* 根据上一笔提交  生成patch */
git format-patch -N1  /

git am     /* 打patch的同时 提交commit信息 */

git apply  /* 打patch的同时 不提交commit信息 */
git apply --check 0001-limit-log-function.patch   // 检查 是否有冲突
git apply getuser.patch  // 打patch


patch -p1 < .../my_patch


//TODO - git 乱码

https://www.cnblogs.com/yanzige/p/9810333.html
git config --global i18n.commitencoding utf-8     /* 注释：该命令表示提交命令的时候使用utf-8编码集提交 */
git config --global i18n.logoutputencoding utf-8  /* 注释：该命令表示日志输出时使用utf-8编码集显示 */
export LESSCHARSET=utf-8                          /* 注释：设置LESS字符集为utf-8 */

