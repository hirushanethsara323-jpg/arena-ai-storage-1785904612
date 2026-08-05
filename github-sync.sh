#!/bin/bash
# GitHub Storage Sync Script
# Repo: https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612

set -e

TOKEN_FILE="$HOME/.netrc"

if [ ! -f "$TOKEN_FILE" ]; then
  echo "⚠️  .netrc file eka na. Token eka aye set karanna one."
  echo "Mehema hadanna:"
  echo '  echo "machine github.com" > ~/.netrc'
  echo '  echo "login YOUR_GITHUB_USERNAME" >> ~/.netrc'
  echo '  echo "password YOUR_NEW_TOKEN" >> ~/.netrc'
  echo '  chmod 600 ~/.netrc'
  exit 1
fi

cd /home/user
echo "📦 Files add karanawa..."
git add .

echo "💾 Commit karanawa..."
if git diff-index --quiet HEAD --; then
  echo "Nothing to commit - already synced"
else
  git commit -m "sync: $(date '+%Y-%m-%d %H:%M')"
fi

echo "🚀 GitHub ekata push karanawa..."
git push origin main

echo "✅ Done! https://github.com/hirushanethsara323-jpg/arena-ai-storage-1785904612"
