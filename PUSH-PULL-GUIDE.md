# GitHub Storage Connected ✅

**Repo:** https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612
**User:** hirushanethsara323-jpg

### Dan kohomada use karanne?

Sandbox eke 128MB limit eka bypass karala, main storage eka GitHub.

#### 1. File ekak haduwama push karanna
```bash
./github-sync.sh
```

#### 2. Pull karanna (anith thanakin hadapu files ganna)
```bash
git pull origin main
```

#### 3. Token eka nathi unoth (restart walata passe)
Arena sandbox eke `.netrc` file eka snapshot eken exclude karala thiyenne security nisa.
Restart ekata passe token eka aye danna one:

```bash
cat > ~/.netrc << NETRC
machine github.com
login hirushanethsara323-jpg
password NEW_TOKEN_HERE
NETRC
chmod 600 ~/.netrc
```

### Security
- Oya kalin dunna token eka (ghp_ZVO...) dan leak wela thiyenne chat eke nisa A-KA REVOKED karanna!
- GitHub > Settings > Developer settings > Personal access tokens > Tokens (classic) > Delete
- Aluth token ekak hadanna: Fine-grained token, 7 days expiry, only this repo Read & Write

### Limit
- GitHub eke single file 100MB walata wada danna ba, eth total repo eka GBs ganan danna puluwan.
