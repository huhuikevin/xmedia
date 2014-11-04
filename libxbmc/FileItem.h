/*!
 \file FileItem.h
 \brief
 */
#pragma once

/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */


#include <vector>


class CURL;

/* special startoffset used to indicate that we wish to resume */
#define STARTOFFSET_RESUME (-1)



enum EFileFolderType {
  EFILEFOLDER_TYPE_ALWAYS     = 1<<0,
  EFILEFOLDER_TYPE_ONCLICK    = 1<<1,
  EFILEFOLDER_TYPE_ONBROWSE   = 1<<2,

  EFILEFOLDER_MASK_ALL        = 0xff,
  EFILEFOLDER_MASK_ONCLICK    = EFILEFOLDER_TYPE_ALWAYS
                              | EFILEFOLDER_TYPE_ONCLICK,
  EFILEFOLDER_MASK_ONBROWSE   = EFILEFOLDER_TYPE_ALWAYS
                              | EFILEFOLDER_TYPE_ONCLICK
                              | EFILEFOLDER_TYPE_ONBROWSE,
};

/*!
  \brief Represents a file on a share
  \sa CFileItemList
  */
class CFileItem :
{
public:
  CFileItem(void);
  CFileItem(const CFileItem& item);
  explicit CFileItem(const std::string& strLabel);
  CFileItem(const CURL& path, bool bIsFolder);
  CFileItem(const std::string& strPath, bool bIsFolder);
  
  virtual ~CFileItem(void);

  const CURL GetURL() const;
  void SetURL(const CURL& url);
  bool IsURL(const CURL& url) const;
  const std::string &GetPath() const { return m_strPath; };
  void SetPath(const std::string &path) { m_strPath = path; };
  bool IsPath(const std::string& path) const;

  /*! \brief reset class to it's default values as per construction.
   Free's all allocated memory.
   \sa Initialize
   */
  void Reset();
  const CFileItem& operator=(const CFileItem& item);

  virtual bool IsFileItem() const { return true; };

  bool Exists(bool bUseCache = true) const;
  


  /* Returns the content type of this item if known */
  const std::string& GetMimeType() const { return m_mimetype; }

  /* sets the mime-type if known beforehand */
  void SetMimeType(const std::string& mimetype) { m_mimetype = mimetype; } ;

 
  void UpdateInfo(const CFileItem &item, bool replaceLabels = true);

  bool IsSamePath(const CFileItem *item) const;



  bool m_bIsShareOrDrive;    ///< is this a root share/drive
  int m_iDriveType;     ///< If \e m_bIsShareOrDrive is \e true, use to get the share type. Types see: CMediaSource::m_iDriveType
//  CDateTime m_dateTime;             ///< file creation date & time
  int64_t m_dwSize;             ///< file size (0 for folders)
  std::string m_strDVDLabel;
  std::string m_strTitle;
  int m_iprogramCount;
  int m_idepth;
  int m_lStartOffset;
  int m_lStartPartNumber;
  int m_lEndOffset;
//  LockType m_iLockMode;
  std::string m_strLockCode;
  int m_iHasLock; // 0 - no lock 1 - lock, but unlocked 2 - locked
  int m_iBadPwdCount;

private:
  /*! \brief initialize all members of this class (not CGUIListItem members) to default values.
   Called from constructors, and from Reset()
   \sa Reset, CGUIListItem
   */
  void Initialize();

  std::string m_strPath;            ///< complete path to item

//  SortSpecial m_specialSort;
  bool m_bIsParentFolder;
  bool m_bCanQueue;
  bool m_bLabelPreformated;
  std::string m_mimetype;
  std::string m_extrainfo;
};

/*!
  \brief A shared pointer to CFileItem
  \sa CFileItem
  */
typedef boost::shared_ptr<CFileItem> CFileItemPtr;

/*!
  \brief A vector of pointer to CFileItem
  \sa CFileItem
  */
typedef std::vector< CFileItemPtr > VECFILEITEMS;

/*!
  \brief Iterator for VECFILEITEMS
  \sa CFileItemList
  */
typedef std::vector< CFileItemPtr >::iterator IVECFILEITEMS;

/*!
  \brief A map of pointers to CFileItem
  \sa CFileItem
  */
typedef std::map<std::string, CFileItemPtr > MAPFILEITEMS;

/*!
  \brief Iterator for MAPFILEITEMS
  \sa MAPFILEITEMS
  */
typedef std::map<std::string, CFileItemPtr >::iterator IMAPFILEITEMS;

/*!
  \brief Pair for MAPFILEITEMS
  \sa MAPFILEITEMS
  */
typedef std::pair<std::string, CFileItemPtr > MAPFILEITEMSPAIR;

typedef bool (*FILEITEMLISTCOMPARISONFUNC) (const CFileItemPtr &pItem1, const CFileItemPtr &pItem2);
typedef void (*FILEITEMFILLFUNC) (CFileItemPtr &item);

/*!
  \brief Represents a list of files
  \sa CFileItemList, CFileItem
  */
class CFileItemList : public CFileItem
{
public:
  enum CACHE_TYPE { CACHE_NEVER = 0, CACHE_IF_SLOW, CACHE_ALWAYS };

  CFileItemList();
  explicit CFileItemList(const std::string& strPath);
  virtual ~CFileItemList();
//  virtual void Archive(CArchive& ar);
  CFileItemPtr operator[] (int iItem);
  const CFileItemPtr operator[] (int iItem) const;
  CFileItemPtr operator[] (const std::string& strPath);
  const CFileItemPtr operator[] (const std::string& strPath) const;
  void Clear();
  void ClearItems();
  void Add(const CFileItemPtr &pItem);
  void AddFront(const CFileItemPtr &pItem, int itemPosition);
  void Remove(CFileItem* pItem);
  void Remove(int iItem);
  CFileItemPtr Get(int iItem);
  const CFileItemPtr Get(int iItem) const;
  const VECFILEITEMS GetList() const { return m_items; }
  CFileItemPtr Get(const std::string& strPath);
  const CFileItemPtr Get(const std::string& strPath) const;
  int Size() const;
  bool IsEmpty() const;
  void Append(const CFileItemList& itemlist);
  void Assign(const CFileItemList& itemlist, bool append = false);
  bool Copy  (const CFileItemList& item, bool copyItems = true);
  void Reserve(int iCount);
//  void Sort(SortBy sortBy, SortOrder sortOrder, SortAttribute sortAttributes = SortAttributeNone);
  /* \brief Sorts the items based on the given sorting options

  In contrast to Sort (see above) this does not change the internal
  state by storing the sorting method and order used and therefore
  will always execute the sorting even if the list of items has
  already been sorted with the same options before.
  */
//  void Sort(SortDescription sortDescription);
  void Randomize();
  void FillInDefaultIcons();
  int GetFolderCount() const;
  int GetFileCount() const;
  int GetSelectedCount() const;
  int GetObjectCount() const;
  void FilterCueItems();
  void RemoveExtensions();
  void SetFastLookup(bool fastLookup);
  bool Contains(const std::string& fileName) const;
  bool GetFastLookup() const { return m_fastLookup; };

  /*! \brief stack a CFileItemList
   By default we stack all items (files and folders) in a CFileItemList
   \param stackFiles whether to stack all items or just collapse folders (defaults to true)
   \sa StackFiles,StackFolders
   */
  void Stack(bool stackFiles = true);

  //SortOrder GetSortOrder() const { return m_sortDescription.sortOrder; }
  //SortBy GetSortMethod() const { return m_sortDescription.sortBy; }
  /*! \brief load a CFileItemList out of the cache

   The file list may be cached based on which window we're viewing in, as different
   windows will be listing different portions of the same URL (eg viewing music files
   versus viewing video files)
   
   \param windowID id of the window that's loading this list (defaults to 0)
   \return true if we loaded from the cache, false otherwise.
   \sa Save,RemoveDiscCache
   */
  //bool Load(int windowID = 0);

  /*! \brief save a CFileItemList to the cache
   
   The file list may be cached based on which window we're viewing in, as different
   windows will be listing different portions of the same URL (eg viewing music files
   versus viewing video files)
   
   \param windowID id of the window that's saving this list (defaults to 0)
   \return true if successful, false otherwise.
   \sa Load,RemoveDiscCache
   */
  //bool Save(int windowID = 0);
  //void SetCacheToDisc(CACHE_TYPE cacheToDisc) { m_cacheToDisc = cacheToDisc; }
  //bool CacheToDiscAlways() const { return m_cacheToDisc == CACHE_ALWAYS; }
  //bool CacheToDiscIfSlow() const { return m_cacheToDisc == CACHE_IF_SLOW; }
  /*! \brief remove a previously cached CFileItemList from the cache
   
   The file list may be cached based on which window we're viewing in, as different
   windows will be listing different portions of the same URL (eg viewing music files
   versus viewing video files)
   
   \param windowID id of the window whose cache we which to remove (defaults to 0)
   \sa Save,Load
   */
  //void RemoveDiscCache(int windowID = 0) const;
  //bool AlwaysCache() const;

  void Swap(unsigned int item1, unsigned int item2);

  /*! \brief Update an item in the item list
   \param item the new item, which we match based on path to an existing item in the list
   \return true if the item exists in the list (and was thus updated), false otherwise.
   */
  bool UpdateItem(const CFileItem *item);

  void SetSortIgnoreFolders(bool sort) { m_sortIgnoreFolders = sort; };
  bool GetReplaceListing() const { return m_replaceListing; };
  void SetReplaceListing(bool replace);
  void SetContent(const std::string &content) { m_content = content; };
  const std::string &GetContent() const { return m_content; };

  void ClearSortState();
private:


  /*!
   \brief stack files in a CFileItemList
   \sa Stack
   */
  void StackFiles();

  /*!
   \brief stack folders in a CFileItemList
   \sa Stack
   */
  void StackFolders();

  VECFILEITEMS m_items;
  //MAPFILEITEMS m_map;
  bool m_fastLookup;
  //SortDescription m_sortDescription;
  bool m_sortIgnoreFolders;
//  CACHE_TYPE m_cacheToDisc;
  bool m_replaceListing;
  std::string m_content;

  std::vector<SORT_METHOD_DETAILS> m_sortDetails;

  CCriticalSection m_lock;
};
