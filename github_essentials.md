# GitHub Essentials: Workflow for Our Project

## **1. Understanding Our Workflow**
We will use a **branch-based workflow** to keep our repository organized and avoid accidental changes in the `main` branch. Every change should go through a feature branch and a **pull request (PR)** before merging into `main`.

### **Basic Steps:**
1. **Pull the latest changes** from `main`.
2. **Create a new branch** for your task.
3. **Make your changes** and commit them.
4. **Push the branch** to GitHub.
5. **Create a pull request (PR)** and request a review.
6. **Merge** the PR into `main` after approval.

---

## **2. Setting Up Your Local Repository**
If you haven't cloned the repository yet, do this:
```sh
# Clone the repository (replace with the actual repo URL)
git clone https://github.com/yourusername/community-donation-platform.git
cd community-donation-platform
```
If you already have the repo, **pull the latest changes** before starting:
```sh
git pull origin main
```

---

## **3. Creating a New Branch**
To keep `main` clean, create a new branch for each feature or fix.
```sh
git checkout -b feature-branch-name
```
Example:
```sh
git checkout -b update-readme
```

---

## **4. Making Changes & Committing**
After modifying files, stage and commit them:
```sh
# Stage changes
git add filename.ext  # Example: git add README.md

# Commit with a meaningful message
git commit -m "Updated README with guidelines"
```

If you've modified multiple files and want to stage everything:
```sh
git add .
git commit -m "Updated multiple files with latest changes"
```

---

## **5. Pushing Changes to GitHub**
Once committed, push your branch to GitHub:
```sh
git push origin feature-branch-name
```
Example:
```sh
git push origin update-readme
```

---

## **6. Creating a Pull Request (PR)**
1. **Go to GitHub** and navigate to our repository.
2. You’ll see a prompt to **Compare & pull request**.
3. Click **Create Pull Request**.
4. Add a description of your changes.
5. Request a review from a teammate.

---

## **7. Reviewing & Merging the PR**
1. Wait for at least **one approval** before merging.
2. Click **Merge Pull Request**.
3. Delete the branch after merging (on GitHub and locally):
```sh
git branch -d feature-branch-name
git push origin --delete feature-branch-name
```

---

## **8. Keeping Your Local Repository Updated**
After merging a PR, always pull the latest changes before starting new work:
```sh
git checkout main
git pull origin main
```

---

## **9. Handling Merge Conflicts**
If Git warns about conflicts when merging, follow these steps:
1. Open the affected file(s) and **manually resolve conflicts**.
2. After resolving, **stage the file**:
```sh
git add conflicted-file.ext
```
3. **Commit the resolved changes**:
```sh
git commit -m "Resolved merge conflicts in conflicted-file.ext"
```
4. **Push changes** and finalize the merge.

---

## **10. Summary: Our Best Practices**
✅ Always create a **new branch** for changes.
✅ **Never push directly to `main`**.
✅ Write **clear commit messages**.
✅ **Pull latest changes** before starting work.
✅ **Review & approve** pull requests before merging.
✅ Delete merged branches to keep the repo clean.

This workflow keeps our project structured and avoids breaking anything on `main`. Let’s follow these steps for a smooth collaboration.

